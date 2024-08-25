#pragma once

#include <stdio.h>
#include <cstdint>

namespace novation
{

#define LAYOUT_CHANGE_CMD 0x00
#define DAW_MODE_FADERS_BANK_CMD 0x01
#define BRIGHTNESS_CMD 0x08
#define LIVE_MODE_CMD 0x0e
#define DAW_MODE_ON_CMD 0x10

#define LAYOUT_MODE_SESSION 0x00
#define LAYOUT_MODE_DRUMS 0x04
#define LAYOUT_MODE_KEYS 0x05
#define LAYOUT_MODE_USER 0x06
#define LAYOUT_MODE_FADERS 0x0d
#define LAYOUT_MODE_PROGRAMMER 0x7f

#define NOTE_ON_MACRO(b2, b3, b4) {0x09, b2, b3, b4}   // programmers mode 0x19, live mode 0x09
#define CONTROL_ON_MACRO(b3, b4) {0x0b, 0xb0, b3, b4}  // Control Changes: B0h (176): Session layout activities.
#define FADER_LEVEL_MACRO(b3, b4) {0x0b, 0xb4, b3, b4} // Channel 5, Control Changes: B4h (180): DAW Fader position sets.
#define FADER_COLOR_MACRO(b3, b4) {0x0b, 0xb5, b3, b4} // Channel 6, Control Changes: B5h (181): DAW Fader colour sets.

#define SHORT_SYSEX_DATA_GET(b9) {0x04, 0xf0, 0x00, 0x20, 0x04, 0x29, 0x02, 0x0d, 0x07, b9, 0xf7}
#define SHORT_SYSEX_DATA_SET(b9, b10) {0x04, 0xf0, 0x00, 0x20, 0x04, 0x29, 0x02, 0x0d, 0x07, b9, b10, 0xf7}
#define SHORT_SYSEX_DATA_BANK(b9, b10) {0x04, 0xf0, 0x00, 0x20, 0x04, 0x29, 0x02, 0x0d, 0x07, b9, 0x0, b10, 0xf7}

// #define SHORT_SYSEX_DATA_GET(b9) {0xf0, 0x00, 0x20, 0x29, 0x02, 0x0d, b9, 0xf7}
// #define SHORT_SYSEX_DATA_SET(b9, b10) {0xf0, 0x00, 0x20, 0x29, 0x02, 0x0d, b9, b10, 0xf7}
// #define SHORT_SYSEX_DATA_BANK(b9, b10) {0xf0, 0x00, 0x20, 0x29, 0x02, 0x0d, b9, 0x0, b10, 0xf7}
    // faders bank
    // f0 00 20 29 02 0d 01   00 00   00 00 15 15    01 00 16 15   02 00 17 15   03 00 18 15    04 00 19 15   05 00 1a 15   06 00 1b 15   07 00 07 1c   15 f7

    // const uint8_t pads_drums[][] = {};
    // const uint8_t pads_keys[][] = {};
    const uint8_t pads_daw[8][8] = {
        {64, 65, 66, 67, 96, 97, 98, 99},
        {60, 61, 62, 63, 92, 93, 94, 95},
        {56, 57, 58, 59, 88, 89, 90, 91},
        {52, 53, 54, 55, 84, 85, 86, 87},
        {48, 49, 50, 51, 80, 81, 82, 83},
        {44, 45, 46, 47, 76, 77, 78, 79},
        {40, 41, 42, 43, 72, 73, 74, 75},
        {36, 37, 38, 39, 68, 69, 70, 71},
    };
    const uint8_t pads_programmer[8][8] = {
        {81, 82, 83, 84, 85, 86, 87, 88},
        {71, 72, 73, 74, 75, 76, 77, 78},
        {61, 62, 63, 64, 65, 66, 67, 68},
        {51, 52, 53, 54, 55, 56, 57, 58},
        {41, 42, 43, 44, 45, 46, 47, 48},
        {31, 32, 33, 34, 35, 36, 37, 38},
        {21, 22, 23, 24, 25, 26, 27, 28},
        {11, 12, 13, 14, 15, 16, 17, 18},
    };

    const uint8_t pads_daw_top[] = {91, 92, 93, 94, 95, 96, 97, 98};
    const uint8_t pads_daw_right[] = {89, 79, 69, 59, 49, 39, 29, 19};

    class Pad
    {
    protected:
        uint8_t id = 0;
        static uint8_t _mode;
        uint8_t _color = 45;
        uint8_t _col = 0;
        uint8_t _row = 0;
        bool _cc = 0;

    public:
        uint8_t _level = 0;

        Pad() = delete;
        Pad(Pad &) = delete;
        explicit Pad(uint8_t col, uint8_t row, bool cc = 0)
        {
            _col = col;
            _row = row;
            _cc = cc;
        }
        virtual void color(uint8_t col)
        {
            _color = col;
        }
        uint8_t color() { return _color; }
        void setMode(uint8_t mode)
        {
            _mode = mode;
        }
        virtual void padPress() = 0;
        virtual void onPadPress() = 0;

        virtual int8_t addr()
        {
            int8_t ret = 0;
            if (_cc == 1 and _col)
            {
                ret = pads_daw_right[_row];
            }
            else if (_cc == 1)
            {
                ret = pads_daw_top[_row];
            }
            else
            {
                ret = pads_programmer[_row][_col];
            }

            return ret;
        }
    };

    class Launchpad
    {
    private:
        uint8_t _channel = 0; // unused

    public:
        Launchpad() = default;
        ~Launchpad() = default;

        uint8_t channel();
        void channel(uint8_t c);
        uint8_t brightness(uint8_t *buf);
        uint8_t brightness(uint8_t *buf, uint8_t lvl); // 0x08

        uint8_t padOff(uint8_t *buf, uint8_t pad);
        uint8_t padOn(uint8_t *buf, uint8_t pad, uint8_t color, uint8_t mode = 0); // 0x19, 0x9m, p, c
        // top/right controls
        uint8_t controlTop(uint8_t *buf, uint8_t n, uint8_t color);
        uint8_t controlRight(uint8_t *buf, uint8_t n, uint8_t color);

        uint8_t liveMode(uint8_t *buf); // 0x0e - 0
        // live/programmer mode 0/1
        uint8_t liveMode(uint8_t *buf, bool on);
        uint8_t dawMode(uint8_t *buf);          // this is broken somehow???
        uint8_t dawMode(uint8_t *buf, bool on); // 0x10
        uint8_t layout(uint8_t *buf);           // 0x00
        // - 00h (0): Session (only selectable in DAW mode)
        // - 04h (4): Custom mode 1 (Drum Rack by factory default)
        // - 05h (5): Custom mode 2 (Keys by factory default)
        // - 06h (6): Custom mode 3 (Lighting mode in Drum Rack layout by factory default)
        // - 0Dh (13): DAW Faders (only selectable in DAW mode)
        // - 7Fh (127): Programmer mode
        uint8_t layout(uint8_t *buf, uint8_t lay); // 0x00
        uint8_t sessionLayout(uint8_t *buf);
        uint8_t drumsLayout(uint8_t *buf);
        uint8_t keysLayout(uint8_t *buf);
        uint8_t userLayout(uint8_t *buf);
        uint8_t fadersLayout(uint8_t *buf);
        uint8_t programmerMode(uint8_t *buf);
        uint8_t faderLevel(uint8_t *buf, uint8_t fdr, uint8_t lvl);
        uint8_t fadersBank(uint8_t *buf);
        uint8_t fadersOrientation(uint8_t *buf, uint8_t horiz = 1);
    };

} // namespace novation

// - in command control payload (0b bx xx xx) third byte is knows as Control Change byte

// faders bank payload is incorrect, orientation is 2 bytes (1 byte in pdf), and there is few extra 0x04 separators

// - inquiry - not known data to send - F0h 7Eh 7Fh 06h 01h F7h
// - SysEx start - 0x04, 0xf0, 0x00, 0x20, 0x04, 0x29, 0x02, 0x0d, 0x07,
// - select layout - get/set - done
// - programmer/live mode - get/set - done
// - surface LEDs - note on/off - mostly done
// - LED light with sysex - not yet
// - DAW on/off - get/set - done
// - DAW in/out - not yet (similar to note on/off) + some extras
// - DAW state clear - not yet
// - DAW faders bank set-up - get/set not yet
// - DAW session colors - get/set - not yet
// - text srolling - not working - ???
// - sleep mode - get/set - not yet
// - brightness - get/set - done
// - LED press feedback - get/set - not yet
// -

/*
const uint8_t red[] = {0x19, 0x90, 0x0b, 0x7f};
const uint8_t dawON[] = {0x04, 0xf0, 0x00, 0x20, 0x04, 0x29, 0x02, 0x0d, 0x07, 0x10, 0x01, 0xF7}; // 0x10
const uint8_t dawOFF[] = {0x04, 0xf0, 0x00, 0x20, 0x04, 0x29, 0x02, 0x0d, 0x07, 0x10, 0xF7};      // 0x10

// SysEx open pattern -
const uint8_t init111[] = {0x04, 0xf0, 0x00, 0x20, 0x04, 0x29, 0x02, 0x0d, 0x07, 1, 7, 0, 37, 72, 101, 104, 104, 111, 32, 87, 111, 114, 104, 100, 247};
const uint8_t init2[] = {0x19, 0x90, 0x0b, 0x01};
const uint8_t init3[] = {0x19, 0x90, 0x0c, 0x01};
const uint8_t init4[] = {0x19, 0x90, 0x0d, 0x01};
const uint8_t init5[] = {0x19, 0x90, 0x0e, 0x01};
const uint8_t init6[] = {0x19, 0x90, 0x0f, 0x01};
const uint8_t init7[] = {0x19, 0x90, 0x10, 0x01};
const uint8_t init8[] = {0x19, 0x90, 0x11, 0x01};
const uint8_t init9[] = {0x19, 0x90, 0x12, 0x01};
const uint8_t init10[] = {0x19, 0x90, 0x15, 0x05};
const uint8_t init11[] = {0x19, 0x90, 0x16, 0x60};
const uint8_t init12[] = {0x19, 0x90, 0x17, 0x0d};
const uint8_t init13[] = {0x19, 0x90, 0x18, 0x59};
const uint8_t init14[] = {0x19, 0x90, 0x19, 0x24};
const uint8_t init15[] = {0x19, 0x90, 0x1a, 0x04};
const uint8_t init16[] = {0x19, 0x90, 0x1b, 0x30};
const uint8_t init17[] = {0x19, 0x90, 0x1c, 0x4e};
const uint8_t init18[] = {0x19, 0x90, 0x1f, 0x05};
const uint8_t init19[] = {0x19, 0x90, 0x20, 0x60};
const uint8_t init20[] = {0x19, 0x90, 0x21, 0x0d};
const uint8_t init21[] = {0x19, 0x90, 0x22, 0x59};
const uint8_t init22[] = {0x19, 0x90, 0x23, 0x24};
const uint8_t init23[] = {0x19, 0x90, 0x24, 0x04};
const uint8_t init24[] = {0x19, 0x90, 0x25, 0x30};
const uint8_t init25[] = {0x19, 0x90, 0x26, 0x4e};
const uint8_t init26[] = {0x19, 0x90, 0x29, 0x05};
const uint8_t init27[] = {0x19, 0x90, 0x2a, 0x60};
const uint8_t init28[] = {0x19, 0x90, 0x2b, 0x0d};
const uint8_t init29[] = {0x19, 0x90, 0x2c, 0x59};
const uint8_t init30[] = {0x19, 0x90, 0x2d, 0x24};
const uint8_t init31[] = {0x19, 0x90, 0x2e, 0x04};
const uint8_t init32[] = {0x19, 0x90, 0x2f, 0x30};
const uint8_t init33[] = {0x19, 0x90, 0x30, 0x4e};
const uint8_t init34[] = {0x19, 0x90, 0x33, 0x05};
const uint8_t init35[] = {0x19, 0x90, 0x34, 0x60};
const uint8_t init36[] = {0x19, 0x90, 0x35, 0x0d, 0x19, 0x90, 0x36, 0x59, 0x19, 0x90, 0x37, 0x24, 0x19, 0x90, 0x38, 0x04};
const uint8_t init37[] = {0x19, 0x90, 0x39, 0x30, 0x19, 0x90, 0x3a, 0x4e, 0x19, 0x90, 0x3d, 0x05, 0x19, 0x90, 0x3e, 0x60};
const uint8_t init38[] = {0x19, 0x90, 0x3f, 0x0d, 0x19, 0x90, 0x40, 0x59, 0x19, 0x90, 0x41, 0x24, 0x19, 0x90, 0x42, 0x04};
const uint8_t init39[] = {0x19, 0x90, 0x43, 0x30, 0x19, 0x90, 0x44, 0x4e, 0x19, 0x90, 0x47, 0x05};
const uint8_t init40[] = {0x19, 0x90, 0x48, 0x60, 0x19, 0x90, 0x49, 0x0d, 0x19, 0x90, 0x4a, 0x59, 0x19, 0x70, 0x4b, 0x24};
const uint8_t init41[] = {0x19, 0x90, 0x4c, 0x04, 0x19, 0x90, 0x4d, 0x30, 0x19, 0x90, 0x4e, 0x4e, 0x19, 0x90, 0x51, 0x03};
const uint8_t init42[] = {0x19, 0x90, 0x52, 0x00, 0x19, 0x90, 0x53, 0x00, 0x19, 0x90, 0x54, 0x00, 0x19, 0x90, 0x55, 0x00};
const uint8_t init43[] = {0x19, 0x90, 0x56, 0x00, 0x19, 0x90, 0x57, 0x00, 0x19, 0x90, 0x58, 0x00, 0x1b, 0xb0, 0x13, 0x7f};
const uint8_t init44[] = {0x1b, 0xb0, 0x1d, 0x7f, 0x1b, 0xb0, 0x27, 0x7f, 0x1b, 0xb0, 0x45, 0x7f, 0x1b, 0xb0, 0x4f, 0x7f};
const uint8_t init45[] = {0x1b, 0xb0, 0x59, 0x7f, 0x1b, 0xb0, 0x5d, 0x7f, 0x1b, 0xb0, 0x5f, 0x7f, 0x1b, 0xb0, 0x60, 0x7f};
const uint8_t init46[] = {0x1b, 0xb0, 0x61, 0x7f, 0x1b, 0xb0, 0x62, 0x7f};

const uint8_t init47[] = {0x1b, 0xb0, 0x5b, 0x7f, 0x1b, 0xb0, 0x5c, 0x7f, 0x1b, 0xb0, 0x5e, 0x7f, 0x1b, 0xb0, 0x3b, 0x7f, 0x1b, 0xb0, 0x31, 0x7f, 0x1b, 0xb0, 0x63, 0x17};
*/