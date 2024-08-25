#pragma once

#include "usb/usb_host.h"
#include "esp_err.h"
#include "esp_event.h"
#include "esp_event_base.h"

#include "device.h"

namespace usb
{
    extern esp_event_loop_handle_t event_loop;

    ESP_EVENT_DECLARE_BASE(USB_HOST_BASE_EVENT);
    class Host
    {
    private:
        usb_host_client_event_cb_t _client_event_cb = nullptr;
        static void _client_event_callback(const usb_host_client_event_msg_t *event_msg, void *arg);

    public:
        Device *devices = nullptr;
        Host() = default;
        ~Host() = default;
        usb_host_client_handle_t client_hdl;

        esp_err_t init(usb_host_client_event_cb_t cb = nullptr, usb_host_enum_filter_cb_t fn = NULL);
        void open();
        void close();
        void addDevice(Device &dev)
        {
            dev.init(client_hdl);
            devices = &dev;
        }
    };

} // namespace usb
