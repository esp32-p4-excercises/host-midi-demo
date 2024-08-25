#include "interface.h"

namespace usb
{

void Interface::init(const usb_config_desc_t* desc, usb_host_client_handle_t client_hdl, usb_device_handle_t dev_hdl, uint8_t bNum, uint8_t bAlt)
{
    bInterfaceNumber = bNum;
    bAlternateSetting = bAlt;
    this->config_desc = desc;
    this->client_hdl = client_hdl;
    this->dev_hdl = dev_hdl;
}

esp_err_t Interface::claim()
{
    if(!client_hdl || ! dev_hdl) return ESP_FAIL;
    esp_err_t err = usb_host_interface_claim(client_hdl, dev_hdl, bInterfaceNumber, bAlternateSetting);
    if (!err)
    {
        parse();
    }
    for (size_t i = 0; i < Count(); i++)
    {
        auto _ep = usb_parse_endpoint_descriptor_by_index(descriptor, i, config_desc->wTotalLength, NULL);
        Endpoint* ep;
        if (USB_EP_DESC_GET_EP_DIR(_ep))
        {
            ep = new EndpointIn();
        } else {
            ep = new EndpointOut();
        }
        ep->init(descriptor, dev_hdl, config_desc->wTotalLength);

        // this->endpoints.push_back(ep);
    }

    return err;
}

esp_err_t Interface::release()
{
    if(!client_hdl || ! dev_hdl) return ESP_FAIL;
    return usb_host_interface_release(client_hdl, dev_hdl, bInterfaceNumber);
}



} // namespace usb
