#pragma once

#include "constants.h"
// #include "midi.h"

#include "usb/usb_types_ch9.h"

// USB_B_DESCRIPTOR_TYPE_CS_ENDPOINT

typedef union {
    struct {
        uint8_t bLength;  
        uint8_t bDescriptorType;  
        uint8_t bDescriptorSubtype;  
        uint8_t bmAttributes;
        uint8_t bLockDelayUnits;  
        uint16_t wLockDelay;
    } USB_DESC_ATTR;  
    uint8_t val[USB_EP_DESC_SIZE];
} audio_ep_desc_t;

ESP_STATIC_ASSERT(sizeof(audio_ep_desc_t) == USB_EP_DESC_SIZE, "Size of audio_ep_desc_t incorrect");

typedef union {
    struct {
        uint8_t bLength;
        uint8_t bDescriptorType;
        uint8_t bInterfaceNumber;
        uint8_t bAlternateSetting;
        uint8_t bNumEndpoints;
        uint8_t bInterfaceClass;
        uint8_t bInterfaceSubClass;
        uint8_t bInterfaceProtocol;
        uint8_t iInterface;
    } USB_DESC_ATTR;
    uint8_t val[USB_INTF_DESC_SIZE];
} usb_midi_intf_desc_t;

