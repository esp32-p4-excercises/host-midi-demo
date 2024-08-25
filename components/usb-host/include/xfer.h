#pragma once

#include "usb/usb_host.h"
#include <esp_log.h>

namespace usb
{
    // static void xfer_callback(usb_transfer_t *transfer) {}

    class Transfer
    {
    protected:
        usb_transfer_t *transfer = NULL;

    public:
        Transfer() = default;
        ~Transfer() = default;

        // virtual void init();
        virtual esp_err_t allocate(size_t len)
        {
            return usb_host_transfer_alloc(len, 0, &transfer);
        };
        virtual void free()
        {
            usb_host_transfer_free(transfer);
        };
        virtual esp_err_t submit(usb_device_handle_t dev_hdl, size_t len)
        {
            transfer->num_bytes = len;
            transfer->device_handle = dev_hdl;
            return usb_host_transfer_submit(transfer);
        };
        virtual uint8_t *buffer() final
        {
            return (uint8_t*)transfer->data_buffer;
        }
        virtual int length() final
        {
            return transfer->actual_num_bytes;
        }
    };

} // namespace usb
