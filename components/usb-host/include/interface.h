#pragma once

#include "stdio.h"
#include <utility>
#include <algorithm>
#include <iostream>
#include <list>
#include <cstddef>
#include <iomanip>
#include <iostream>
#include <memory>
#include <utility>

#include "esp_err.h"

#include "usb/usb_host.h"
#include "descriptor.h"
#include "endpoint.h"

namespace usb
{
    class Interface : Descriptor<usb_intf_desc_t>
    {
    private:
        const usb_config_desc_t* config_desc;
        uint8_t bInterfaceNumber;
        uint8_t bAlternateSetting;
        usb_host_client_handle_t client_hdl;
        usb_device_handle_t dev_hdl;
        std::list<Endpoint*> endpoints;
        
    public:
        Interface() = default;
        ~Interface() = default;

        virtual void init(const usb_config_desc_t *config_desc, usb_host_client_handle_t client_hdl, usb_device_handle_t dev_hdl, uint8_t bNum, uint8_t bAlt = 0);
        virtual esp_err_t claim();
        virtual esp_err_t release();

        virtual void parse() {
            descriptor = usb_parse_interface_descriptor(config_desc, bInterfaceNumber, bAlternateSetting, NULL);
        }

        virtual inline int8_t Count() final {
            if(!descriptor) return -1;
            return descriptor->bNumEndpoints;
        }
        virtual inline int8_t Class() final {
            if(!descriptor) return -1;
            return descriptor->bInterfaceClass;
        }
        virtual inline int8_t Subclass() final {
            if(!descriptor) return -1;
            return descriptor->bInterfaceSubClass;
        }
        virtual inline int8_t Protocol() final {
            if(!descriptor) return -1;
            return descriptor->bInterfaceProtocol;
        }
        virtual void findByType();
        virtual void findNext();

    };
} // namespace usb
