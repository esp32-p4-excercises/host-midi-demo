#include <tuple>

#include "sys/cdefs.h"

#include "device.h"
#include <esp_err.h>
#include <usb/usb_host.h>
#include <esp_log.h>
#include "usb_requests.h"
#include "host.h"

namespace usb
{

    void Device::_client_event_callback(const usb_host_client_event_msg_t *event_msg, void *arg)
    {
        Device *device = (Device *)arg;
        if (event_msg->event == USB_HOST_CLIENT_EVENT_NEW_DEV)
        {
            ESP_LOGI("USB_HOST_CLIENT_EVENT_NEW_DEV", "client event: %d, address: %d", event_msg->event, event_msg->new_dev.address);
            auto tuple = std::make_tuple(USB_HOST_CLIENT_EVENT_NEW_DEV, device);
            device->open(event_msg->new_dev.address);
            esp_event_post_to(usb::event_loop, USB_HOST_BASE_EVENT, USB_HOST_CLIENT_EVENT_NEW_DEV, &tuple, sizeof(tuple), 10);
        }
        else
        {
            ESP_LOGI("USB_HOST_CLIENT_EVENT_DEV_GONE", "client event: %d", event_msg->event);
            device->close();
            esp_event_post_to(usb::event_loop, USB_HOST_BASE_EVENT, USB_HOST_CLIENT_EVENT_DEV_GONE, device, 1, 10);
        }
    }

    void Device::init(usb_host_client_handle_t hdl)
    {
        const usb_host_client_config_t client_config = {
            .max_num_event_msg = 5,
            .async = {
                .client_event_callback = this->_client_event_callback,
                .callback_arg = this,
            }};

        auto err = usb_host_client_register(&client_config, &client_hdl);
        ESP_LOGI("", "client register status: %s", esp_err_to_name(err));
    }

    void Device::claim(uint8_t itf)
    {
        itf_num = itf;
        esp_err_t err = usb_host_interface_claim(client_hdl, dev_hdl, itf_num, 0);
        ESP_LOGI("", "interface %d claim status: %s", itf_num, esp_err_to_name(err));
    }

    void Device::release()
    {
        // esp_err_t usb_host_interface_release(usb_host_client_handle_t client_hdl, usb_device_handle_t dev_hdl, uint8_t bInterfaceNumber);
    }

} // namespace usb
