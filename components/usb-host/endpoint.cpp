#include "endpoint.h"

namespace usb
{
    esp_err_t Endpoint::init(const usb_intf_desc_t *intf_desc, usb_device_handle_t dev_hdl, int len)
    {
        this->intf_desc = intf_desc;
        wTotalLength = len;
        parse();
        this->dev_hdl = dev_hdl;

        esp_err_t err = allocate(mps());
        if(err == ESP_OK)
        {
            transfer->device_handle = dev_hdl;
            transfer->bEndpointAddress = descriptor->bEndpointAddress;
        }

        return err;
    }

    esp_err_t Endpoint::reallocate(size_t len, void* ctx)
    {
        esp_err_t err = ESP_OK;
        if(transfer->num_bytes >= len) return ESP_OK;
        free();
        err = allocate(len);
        if(err == ESP_OK)
        {
            transfer->device_handle = dev_hdl;
            transfer->bEndpointAddress = descriptor->bEndpointAddress;
            context(ctx);
        }

        return err;
    }

    void Endpoint0::usb_transfer_cb(usb_transfer_t *transfer)
    {
        printf("EP0 xfer callback\n");
            ESP_LOG_BUFFER_HEX("EP0", transfer, 18);
        esp_event_post(USB_HOST_BASE_EVENT, USB_HOST_EP0_XFER, transfer, sizeof(usb_transfer_t*), 0);
    }

} // namespace usb
