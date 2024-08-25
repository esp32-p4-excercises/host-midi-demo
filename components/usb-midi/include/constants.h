#pragma once
#include <stdio.h>
// https://www.usb.org/sites/default/files/audio10.pdf

// note AC - audio control interface (0 epoints?), MS - midi streaming interface
#define INTERFACE_CLASS_AUDIO   0x01

#define INTERFACE_SUBCLASS_UNDEFINED                0x00
#define INTERFACE_SUBCLASS_AUDIOCONTROL             0x01
#define INTERFACE_SUBCLASS_AUDIOSTREAMING           0x02
#define INTERFACE_SUBCLASS_MIDISTREAMING            0x03


#define USB_B_DESCRIPTOR_TYPE_CS_UNDEFINED          0x20
#define USB_B_DESCRIPTOR_TYPE_CS_DEVICE             0x21
#define USB_B_DESCRIPTOR_TYPE_CS_CONFIGURATION      0x22
#define USB_B_DESCRIPTOR_TYPE_CS_STRING             0x23
#define USB_B_DESCRIPTOR_TYPE_CS_INTERFACE          0x24
#define USB_B_DESCRIPTOR_TYPE_CS_ENDPOINT           0x25

// A.1 MS Class-Specific Interface Descriptor Subtypes
#define MS_DESCRIPTOR_UNDEFINED 0x00
#define MS_HEADER 0x01
#define MIDI_IN_JACK 0x02
#define MIDI_OUT_JACK 0x03
#define ELEMENT 0x04
// A.2 MS Class-Specific Endpoint Descriptor Subtypes
#define DESCRIPTOR_UNDEFINED 0x00
#define MS_GENERAL 0x01
// A.3 MS MIDI IN and OUT Jack types
#define JACK_TYPE_UNDEFINED 0x00
#define EMBEDDED 0x01
#define EXTERNAL 0x02
// A.5.1 Endpoint Control Selectors
#define EP_CONTROL_UNDEFINED 0x00
#define ASSOCIATION_CONTROL 0x01






// midi packet is 4 bytes length

// byte1 - cable number + CIN
// byte2 - byte4  

// CIN - low nimble
#define CIN_02          // Two-byte System Common messages like MTC, SongSelect, etc.
#define CIN_03          // Three-byte System Common messages like SPP, etc.
#define CIN_04          // SysEx starts or continues
#define CIN_05          // Single-byte System Common Message or SysEx ends with following single byte
#define CIN_06          // SysEx ends with following two bytes.
#define CIN_07          // SysEx ends with following three bytes.
#define CIN_08          // Note-off
#define CIN_09          // Note-on
#define CIN_0A          // Poly-KeyPress
#define CIN_0B          // Control Change
#define CIN_0C          // Program Change
#define CIN_0D          // Channel Pressure
#define CIN_0E          // PitchBend Change
#define CIN_0F          // Single Byte


