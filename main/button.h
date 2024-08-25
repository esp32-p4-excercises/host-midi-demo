#pragma once

#include "lvgl.h"
#include "misc/lv_style.h"

#include "usb_midi.h"
#include "launchpad-mini-mk3.h"

class Button : public novation::Pad
{
protected:
    lv_obj_t *btn = nullptr;
    static usb::MIDI *midi;
    uint8_t buf[64] = {};
    lv_obj_t *grid = nullptr;

public:
    using Pad::Pad;
    void init(lv_event_cb_t event_cb, lv_obj_t *obj);
    static void setMIDI(usb::MIDI *midi)
    {
        Button::midi = midi;
    }
    void style(lv_style_t *style);
    void style(lv_style_t *old_style, lv_style_t *new_style);
    void position(int col, int row, const char *txt)
    {
        lv_obj_set_grid_cell(btn, LV_GRID_ALIGN_STRETCH, col, 1, LV_GRID_ALIGN_STRETCH, row, 1);
        auto label = lv_label_create(btn);
        lv_label_set_text(label, txt);
        lv_obj_center(label);
    }
    virtual void color(uint8_t c) override
    {
        Pad::color(c);
        static const uint32_t colors[] = {
            0x000000, 0x505050, 0xa0a0a0, 0xffffff, 0xff9185, 0xff230a, 0xe61f09, 0xcc1c08,
            0xffc285, 0xff850a, 0xe67709, 0xcc6a08, 0xfff385, 0xffe70a, 0xe6cf09, 0xccb808,
            0xdaff85, 0xb6ff0a, 0xa3e609, 0x91cc08, 0xa9ff85, 0x54ff0a, 0x4be609, 0x43cc08,
            0x85ff91, 0x0aff23, 0x09e61f, 0x08cc1c, 0x85ffc2, 0x0aff85, 0x09e677, 0x08cc6a,
            0x85fff3, 0x0affe7, 0x09e6cf, 0x08ccb8, 0x85daff, 0x0ab6ff, 0x09a3e6, 0x0891cc,
            0x85a9ff, 0x0a54ff, 0x094be6, 0x0843cc, 0x9185ff, 0x230aff, 0x1f09e6, 0x1c08cc,
            0xc285ff, 0x850aff, 0x7709e6, 0x6a08cc, 0xf385ff, 0xe70aff, 0xcf09e6, 0xb808cc,
            0xff85da, 0xff0ab6, 0xe609a3, 0xcc0891, 0xf04115, 0xbf6100, 0xb18c00, 0x859708,
            0x50a027, 0x009d8e, 0x0079c0, 0x0000ff, 0x2d50a4, 0x6247b0, 0x7b7b7b, 0x3c3c3c,
            0xff0000, 0xbfbb64, 0xa6c000, 0x78c823, 0x34c500, 0x00c0af, 0x00a2f1, 0x527de7,
            0x8868e7, 0xa447af, 0xb93b69, 0x975731, 0xf86c00, 0xbefd00, 0x82ff5d, 0x00ff00,
            0x00ffa5, 0x52ffe8, 0x00e9ff, 0x89c4ff, 0x91a5ff, 0xb989ff, 0xda67e7, 0xff2cd6,
            0xffa601, 0xfff200, 0xe3f600, 0xdcc500, 0xbf9e5f, 0x88b57b, 0x86c2ba, 0x9ab3c5,
            0x84a5c3, 0xc78b7a, 0xf43c7f, 0xff93a5, 0xffa36f, 0xffef9a, 0xd2e594, 0xbad16f,
            0xa9a9a9, 0xd3fee0, 0xccf1f9, 0xb9c0e4, 0xcdbae5, 0xd0d0d0, 0xdfe6e5, 0xffffff,
            0xf2210a, 0xbf1a08, 0xacf20a, 0x88bf08, 0xf2db0a, 0xbfad08, 0xf7a409, 0xe06b41,
        };
        auto col = lv_color_hex(colors[c]);
        lv_obj_set_style_bg_color(btn, col, LV_PART_MAIN | LV_STATE_DEFAULT);
    }

    virtual void onPadPress() override;
    virtual void onRelease();
    virtual void padPress() override {};
};

class DAWbutton : public Button
{
private:
    bool daw = false;
    bool _cc = true;

public:
    using Button::Button;
    virtual void onPadPress() override;
    virtual void onRelease() override;
    void changeMode();
};

class Faders : public Button
{
private:
    bool daw = false;
    bool _cc = true;

public:
    using Button::Button;
    virtual void onPadPress() override;
    virtual void onRelease() override;
    void changeMode();
};

class Session : public Button
{
private:
public:
    using Button::Button;
    virtual void onPadPress() override;
    virtual void onRelease() override;
    void changeMode();
};

class Drums : public Button
{
private:
public:
    using Button::Button;
    virtual void onPadPress() override;
    virtual void onRelease() override;
    void changeMode();
};

class Keys : public Button
{
private:
public:
    using Button::Button;
    virtual void onPadPress() override;
    virtual void onRelease() override;
    void changeMode();
};

class User : public Button
{
private:
public:
    using Button::Button;
    virtual void onPadPress() override;
    virtual void onRelease() override;
    void changeMode();
};
