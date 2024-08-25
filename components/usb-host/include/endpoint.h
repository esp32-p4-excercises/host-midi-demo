#pragma once

#include "stdio.h"
#include "esp_event.h"
#include "esp_event_base.h"

#include "xfer.h"
#include "descriptor.h"
#include "esp_log.h"
namespace usb
{
ESP_EVENT_DECLARE_BASE(USB_HOST_BASE_EVENT);

#define USB_HOST_EP0_XFER       0x10001
#define USB_HOST_MAX_LUNS       0x10002
#define USB_HOST_MSC_READY      0x10003
#define USB_HOST_MSC_INQUIRY        0x10004
#define USB_HOST_MSC_CAPACITY       0x10005

    class Endpoint : public Transfer, Descriptor<usb_ep_desc_t>
    {
    private:
        const usb_intf_desc_t *intf_desc = nullptr;
        usb_device_handle_t dev_hdl = nullptr;
        int index = 0;
        uint16_t wTotalLength = 0;

    public:
        Endpoint() = default;
        ~Endpoint() = default;

        void init(const usb_ep_desc_t *desc)
        {
            descriptor = desc;
            allocate(mps());
            transfer->bEndpointAddress = descriptor->bEndpointAddress;
        };
        esp_err_t init(const usb_intf_desc_t *intf_desc, usb_device_handle_t dev_hdl, int len);
        esp_err_t reallocate(size_t len, void* ctx = nullptr);
        virtual int mps()
        {
            assert(descriptor);
            return USB_EP_DESC_GET_MPS(descriptor);
        }

        virtual void context(void* ctx) {
            transfer->context = ctx;
        }

        virtual void callback(usb_transfer_cb_t cb)
        {
            transfer->callback = cb;
        }

    private:
        inline void parse()
        {
            int offset;
            descriptor = usb_parse_endpoint_descriptor_by_index(intf_desc, index, wTotalLength, &offset);
        }
    };

    class Endpoint0 : public Endpoint
    {
    private:
        usb_host_client_handle_t client_hdl;
        static void usb_transfer_cb(usb_transfer_t *transfer);

    public:
        Endpoint0() = default;
        ~Endpoint0() = default;

        virtual esp_err_t init(usb_host_client_handle_t client_hdl)
        {
            this->client_hdl = client_hdl;
            auto err = allocate(256);
            transfer->callback = this->usb_transfer_cb;
            transfer->num_bytes = 0;
            return err;
        };
        virtual esp_err_t submit(usb_device_handle_t dev_hdl, size_t len) override
        {
            transfer->device_handle = dev_hdl;
            transfer->num_bytes = len;
            // printf("submit ep0 bytes: %d\n", transfer->num_bytes);
            // ESP_LOG_BUFFER_HEX("submit", transfer, 18);
            return usb_host_transfer_submit_control(client_hdl, transfer);
        }
    };

    class EndpointIn : public Endpoint
    {
    private:
    public:
        EndpointIn() = default;
        ~EndpointIn() = default;

        // virtual esp_err_t receive(size_t len)
        // {
        //     transfer->num_bytes = len;
        //     return submit();
        // }
    };

    class EndpointOut : public Endpoint
    {
    private:
    public:
        EndpointOut() = default;
        ~EndpointOut() = default;

        // virtual esp_err_t send(uint8_t data, size_t len)
        // {
        //     transfer->num_bytes = len;
        //     // memcpy(transfer->data_buffer, data, len);

        //     return submit();
        // }
    };

} // namespace usb
