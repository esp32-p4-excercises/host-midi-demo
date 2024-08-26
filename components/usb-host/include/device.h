#pragma once

#include <utility>
#include <algorithm>
#include <iostream>
#include <list>
#include "esp_event.h"
#include "esp_event_base.h"

#include "usb/usb_host.h"
#include "esp_log.h"
#include "interface.h"
#include "freertos/task.h"

using namespace std;
namespace usb
{
    ESP_EVENT_DECLARE_BASE(USB_HOST_BASE_EVENT);

    class Device
    {
    protected:
        usb_device_handle_t dev_hdl = nullptr;
        usb_host_client_handle_t client_hdl = nullptr;
        const usb_config_desc_t *config_desc = nullptr;
        const usb_device_desc_t *device_desc = nullptr;

        // TODO
        const usb_intf_desc_t *intf = nullptr;
        std::list<const usb_intf_desc_t *> interfaces;
        Endpoint0 *ep0;
        int address = 0;
        int bInterfaceClass = 0;
        int bInterfaceSubClass = 0;
        int bInterfaceProtocol = 0;
        int itf_num = -1;

    public:
        Device() {};
        Device(Device &) = default;
        virtual ~Device() = default;
        static void _client_event_callback(const usb_host_client_event_msg_t *event_msg, void *arg);

        virtual void init(usb_host_client_handle_t hdl);
        virtual Endpoint0 *Ep0() final
        {
            return ep0;
        }

        virtual void open(int addr)
        {
            address = addr;
            esp_err_t err = usb_host_device_open(client_hdl, address, &dev_hdl);
            ESP_ERROR_CHECK_WITHOUT_ABORT(err);
            ep0 = new Endpoint0();
            err = ep0->init(client_hdl);
            ESP_ERROR_CHECK(err);
        };
        virtual void event()
        {
            if (client_hdl)
                usb_host_client_handle_events(client_hdl, portMAX_DELAY);
            else
            {
                vTaskDelay(1);
            }
        }
        virtual void dereg()
        {
            usb_host_interface_release(client_hdl, dev_hdl, itf_num);
            usb_host_device_close(client_hdl, dev_hdl);
            auto err = usb_host_client_deregister(client_hdl);
            ESP_ERROR_CHECK_WITHOUT_ABORT(err);
            printf("close: %s\n", esp_err_to_name(err));
            client_hdl = NULL;
        }
        virtual void close()
        {
            esp_err_t err = usb_host_device_close(client_hdl, dev_hdl);
            ESP_ERROR_CHECK_WITHOUT_ABORT(err);
        };
        virtual void deviceDesc()
        {
            esp_err_t err = usb_host_get_device_descriptor(dev_hdl, &device_desc);
            ESP_ERROR_CHECK_WITHOUT_ABORT(err);
            ESP_LOG_BUFFER_HEX("", device_desc->val, USB_DEVICE_DESC_SIZE);
        };
        virtual void configDesc()
        {
            esp_err_t err = usb_host_get_active_config_descriptor(dev_hdl, &config_desc);
            ESP_ERROR_CHECK_WITHOUT_ABORT(err);
            ESP_LOG_BUFFER_HEX("", config_desc->val, USB_CONFIG_DESC_SIZE);
        };
        virtual usb_device_info_t info()
        {
            usb_device_info_t dev_info;
            usb_host_device_info(dev_hdl, &dev_info);
            return dev_info;
        }
        virtual void claim() { abort(); };
        virtual void claim(uint8_t itf_num) final;
        virtual void release() final;

        virtual usb_device_handle_t dev_handle() final
        {
            return dev_hdl;
        }
        virtual usb_host_client_handle_t client_handle() final
        {
            return client_hdl;
        }

        static void print_class_descriptor(const usb_standard_desc_t *desc)
        {
            printf("some descriptor: 0x%02x\n\n\n", desc->bDescriptorType);
        }
        void print()
        {

            const usb_config_desc_t *config_desc;
            usb_host_get_active_config_descriptor(dev_hdl, &config_desc);
            usb_print_config_descriptor(config_desc, print_class_descriptor);
        }

        bool connected()
        {
            auto itf = Interface();
            itf.init(config_desc, client_hdl, dev_hdl, itf_num);
            itf.parse();
            return itf.Class() == bInterfaceClass and itf.Subclass() == bInterfaceSubClass and itf.Protocol() == bInterfaceProtocol;
        }
    };

} // namespace usb
