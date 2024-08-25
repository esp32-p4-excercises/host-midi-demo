#pragma once

#include "device.h"

#include "usb_requests.h"
#include "constants.h"

typedef struct usb_midi
{
    uint8_t cmd; // cable + command
    uint8_t data1;
    uint8_t data2;
    uint8_t data3;
} usb_midi_data_t;

namespace usb
{
    ESP_EVENT_DECLARE_BASE(USB_MIDI_BASE_EVENT);

#define USB_MIDI_DATA_IN 0x01

    class MIDI : public Device
    {
    private:
        const usb_ep_desc_t *ep_in;
        const usb_ep_desc_t *ep_out;
        int xfer_status = 0;
        Endpoint xferIn;
        Endpoint xferOut;

    public:
        MIDI() = delete;
        MIDI(Device &dev)
        {
            dev_hdl = dev.dev_handle();
            client_hdl = dev.client_handle();
            Device::bInterfaceClass = 0x01;
            Device::bInterfaceSubClass = 0x03;
            ep0 = dev.Ep0();
        }

        ~MIDI() = default;

        virtual void claim() override;
        virtual std::tuple<int, uint8_t *> read();
        virtual int read(uint8_t* buf, size_t len);
        virtual void write(const uint8_t *data, size_t len);
    };

} // namespace usb
