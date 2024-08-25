#include <string.h>

#include "launchpad-mini-mk3.h"

namespace novation
{
    uint8_t Pad::_mode = 0;

    uint8_t Launchpad::channel()
    {
        return _channel;
    }

    void Launchpad::channel(uint8_t c)
    {
        _channel = c;
    }

    uint8_t Launchpad::padOff(uint8_t *buf, uint8_t pad)
    {
        assert(buf);
        uint8_t note[] = NOTE_ON_MACRO(0x90, pad, 0x0);
        memcpy(buf, note, 4);
        return 4;
    }

    uint8_t Launchpad::padOn(uint8_t *buf, uint8_t pad, uint8_t color, uint8_t channel)
    {
        assert(buf);
        uint8_t note[] = NOTE_ON_MACRO((uint8_t)(0x90 + channel), pad, color);
        memcpy(buf, note, 4);
        return 4;
    }

    uint8_t Launchpad::controlTop(uint8_t *buf, uint8_t n, uint8_t color)
    {
        assert(buf);
        uint8_t note[] = CONTROL_ON_MACRO((uint8_t)(0x5b + n), color);
        memcpy(buf, note, 4);
        return 4;
    }

    uint8_t Launchpad::controlRight(uint8_t *buf, uint8_t n, uint8_t color)
    {
        assert(buf);
        uint8_t byte = 0x63 - 10 * n;
        uint8_t note[] = CONTROL_ON_MACRO(byte, color);
        memcpy(buf, note, 4);
        return 4;
    }

    uint8_t Launchpad::brightness(uint8_t *buf)
    {
        assert(buf);
        uint8_t mode[] = SHORT_SYSEX_DATA_GET(BRIGHTNESS_CMD); // 0x08
        memcpy(buf, mode, sizeof(mode));
        return sizeof(mode);
    }

    uint8_t Launchpad::brightness(uint8_t *buf, uint8_t lvl)
    {
        assert(buf);
        uint8_t mode[] = SHORT_SYSEX_DATA_SET(BRIGHTNESS_CMD, lvl); // 0x08
        memcpy(buf, mode, sizeof(mode));
        return sizeof(mode);
    }

    uint8_t Launchpad::liveMode(uint8_t *buf)
    {
        assert(buf);
        uint8_t mode[] = SHORT_SYSEX_DATA_GET(LIVE_MODE_CMD);
        memcpy(buf, mode, sizeof(mode));
        return sizeof(mode);
    }

    uint8_t Launchpad::liveMode(uint8_t *buf, bool on)
    {
        assert(buf);
        uint8_t mode[] = SHORT_SYSEX_DATA_SET(LIVE_MODE_CMD, on);
        memcpy(buf, mode, sizeof(mode));
        return sizeof(mode);
    }

    uint8_t Launchpad::dawMode(uint8_t *buf)
    {
        assert(buf);
        uint8_t mode[] = SHORT_SYSEX_DATA_GET(DAW_MODE_ON_CMD);
        memcpy(buf, mode, sizeof(mode));
        return sizeof(mode);
    }

    uint8_t Launchpad::dawMode(uint8_t *buf, bool on)
    {
        assert(buf);
        uint8_t mode[] = SHORT_SYSEX_DATA_SET(DAW_MODE_ON_CMD, on);
        memcpy(buf, mode, sizeof(mode));
        return sizeof(mode);
    }

    uint8_t Launchpad::layout(uint8_t *buf)
    {
        assert(buf);
        uint8_t mode[] = SHORT_SYSEX_DATA_GET(LAYOUT_CHANGE_CMD);
        memcpy(buf, mode, sizeof(mode));
        return sizeof(mode);
    }

    uint8_t Launchpad::layout(uint8_t *buf, uint8_t lay)
    {
        assert(buf);
        uint8_t mode[] = SHORT_SYSEX_DATA_SET(LAYOUT_CHANGE_CMD, lay);
        memcpy(buf, mode, sizeof(mode));
        return sizeof(mode);
    }

    uint8_t Launchpad::sessionLayout(uint8_t *buf)
    {
        return layout(buf, LAYOUT_MODE_SESSION);
    }

    uint8_t Launchpad::drumsLayout(uint8_t *buf)
    {
        return layout(buf, LAYOUT_MODE_DRUMS);
    }

    uint8_t Launchpad::keysLayout(uint8_t *buf)
    {
        return layout(buf, LAYOUT_MODE_KEYS);
    }

    uint8_t Launchpad::userLayout(uint8_t *buf)
    {
        return layout(buf, LAYOUT_MODE_USER);
    }

    uint8_t Launchpad::fadersLayout(uint8_t *buf)
    {
        return layout(buf, LAYOUT_MODE_FADERS);
    }

    uint8_t Launchpad::programmerMode(uint8_t *buf)
    {
        return layout(buf, LAYOUT_MODE_PROGRAMMER);
    }

    uint8_t Launchpad::faderLevel(uint8_t *buf, uint8_t fdr, uint8_t lvl)
    {
        assert(buf);
        uint8_t fader[] = FADER_LEVEL_MACRO((uint8_t)(0x15 + fdr), lvl);
        memcpy(buf, fader, 4);
        return 4;
    }

    uint8_t Launchpad::fadersBank(uint8_t *buf)
    {
        assert(buf);
        uint8_t mode[] = SHORT_SYSEX_DATA_GET(DAW_MODE_FADERS_BANK_CMD);
        memcpy(buf, mode, sizeof(mode));
        return sizeof(mode);
    }

    uint8_t Launchpad::fadersOrientation(uint8_t *buf, uint8_t horiz)
    {
        assert(buf);
        uint8_t mode[] = SHORT_SYSEX_DATA_BANK(DAW_MODE_FADERS_BANK_CMD, horiz);
        memcpy(buf, mode, sizeof(mode));
        return sizeof(mode);
    }
} // namespace novation
