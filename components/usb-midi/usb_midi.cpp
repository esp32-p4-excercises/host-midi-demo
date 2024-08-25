#include <cstring>

#include "usb_midi.h"
#include "host.h"

void midi_event_handler(void *event_handler_arg, esp_event_base_t event_base, int32_t event_id, void *arg);

namespace usb
{
    ESP_EVENT_DEFINE_BASE(USB_MIDI_BASE_EVENT);

    SemaphoreHandle_t transfer_in = nullptr;

    static void usb_transfer_in(usb_transfer_t *transfer)
    {
        xSemaphoreGive(transfer_in);
    }
    static void usb_transfer_out(usb_transfer_t *transfer)
    {
    }

    void MIDI::claim()
    {

        transfer_in = xSemaphoreCreateBinary();
        xSemaphoreGive(transfer_in);

        if (!config_desc)
            configDesc();
        int offset = 0;
        for (size_t n = 0; n < config_desc->bNumInterfaces; n++)
        {
            const usb_intf_desc_t *intf = usb_parse_interface_descriptor(config_desc, n, 0, &offset);
            const usb_ep_desc_t *ep = nullptr;

            if (intf->bInterfaceClass == bInterfaceClass and intf->bInterfaceSubClass == bInterfaceSubClass)
            {
                auto itf_num = n;
                this->intf = intf;
                Device::claim(itf_num);
                for (size_t i = 0; i < intf->bNumEndpoints; i++)
                {
                    int _offset = 0;
                    ep = usb_parse_endpoint_descriptor_by_index(intf, i, config_desc->wTotalLength, &_offset);
                    if (USB_EP_DESC_GET_EP_DIR(ep))
                    {
                        ep_in = ep;
                        xferIn = Endpoint();
                        xferIn.init(ep_in);
                        xferIn.callback(usb_transfer_in);
                        xferIn.context(this);
                    }
                    else
                    {
                        ep_out = ep;
                        xferOut = Endpoint();
                        xferOut.init(ep_out);
                        xferOut.callback(usb_transfer_out);
                        xferOut.context(this);
                    }

                    if (ep)
                    {
                        ESP_LOGI("MIDI", "claim EP num: %d/%d, len: %d, ", i + 1, intf->bNumEndpoints, config_desc->wTotalLength);
                        ESP_LOGI("MIDI", "claim address: 0x%02x, EP max size: %d, dir: %s\n", ep->bEndpointAddress, ep->wMaxPacketSize, (ep->bEndpointAddress & 0x80) ? "IN" : "OUT");
                    }
                    else
                        ESP_LOGW("", "error to parse endpoint by index; EP num: %d/%d, len: %d", i + 1, intf->bNumEndpoints, config_desc->wTotalLength);
                }
            }
        }
    }

    std::tuple<int, uint8_t *> MIDI::read()
    {
        if (xSemaphoreTake(transfer_in, portMAX_DELAY))
        {
            auto err = xferIn.submit(dev_hdl, ep_in->wMaxPacketSize);
            ESP_ERROR_CHECK_WITHOUT_ABORT(err);
        }
        if (xSemaphoreTake(transfer_in, 3000))
        {
            auto buf = xferIn.buffer();
            auto len = xferIn.length();
            auto tpl = std::tuple<int, uint8_t *>(len, buf);
            xSemaphoreGive(transfer_in);

            return tpl;
        }

        return std::tuple<int, uint8_t *>(0, NULL);
    }

    int MIDI::read(uint8_t *buf, size_t len)
    {
        assert(buf);
        auto _len = usb_round_up_to_mps(len, ep_in->wMaxPacketSize);
        if (xSemaphoreTake(transfer_in, portMAX_DELAY))
        {
            auto err = xferIn.submit(dev_hdl, _len);
            ESP_ERROR_CHECK_WITHOUT_ABORT(err);
        }
        if (xSemaphoreTake(transfer_in, 3000))
        {
            auto _buf = xferIn.buffer();
            auto __len = xferIn.length();
            memcpy(buf, _buf, __len);
            xSemaphoreGive(transfer_in);

            return __len;
        }

        return 0;
    }

    void MIDI::write(const uint8_t *data, size_t len)
    {

        auto buf = xferOut.buffer();
        memcpy(buf, data, len);
        xferOut.submit(dev_hdl, len);
    }

} // namespace usb
