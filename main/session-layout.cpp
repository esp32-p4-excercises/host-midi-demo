#include <map>

#include "esp_log.h"
#include "esp_err.h"
#include "lvgl.h"
#include "misc/lv_style.h"

#include "button.h"

#include "usb_midi.h"
#include "launchpad-mini-mk3.h"
#include "host.h"

using namespace usb;

#define BUTTON_WIDTH 60
#define BUTTON_HEIGTH 60

void midi_event_btn(void *p);

static lv_style_t style_btn1;
static lv_style_t style_btn2;

extern usb::MIDI *midi;
novation::Launchpad pad;
static DAWbutton dawMode(11, 0, true);
static Faders fadersMode(11, 1, true);
static Session session(9, 4, true);
static Drums drums(9, 5, true);
static Keys keys(9, 6, true);
static User user(9, 7, true);

std::map<int, Button *> buttons_map;
static lv_obj_t *cont = nullptr;

static void btn_event_cb(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    auto btnClass = (Button *)lv_event_get_user_data(e);
    if (!btnClass)
        return;
    switch (code)
    {
    case LV_EVENT_PRESSED:
    {
        btnClass->onPadPress();
        break;
    }
    case LV_EVENT_RELEASED:
    {
        btnClass->onRelease();
        break;
    }
    default:
        break;
    }
}

static void lv_example_grid(void)
{
    static int32_t col_dsc[] = {60, 60, 60, 60, 60, 60, 60, 60, 190, 60, 60, 60, LV_GRID_TEMPLATE_LAST};
    static int32_t row_dsc[] = {60, 60, 60, 60, 60, 60, 60, 60, LV_GRID_TEMPLATE_LAST};

    /*Create a container with grid*/
    cont = lv_obj_create(lv_screen_active());
    lv_obj_set_style_grid_column_dsc_array(cont, col_dsc, 0);
    lv_obj_set_style_grid_row_dsc_array(cont, row_dsc, 0);
    lv_obj_set_size(cont, 1024, 600);
    lv_obj_center(cont);
    lv_obj_set_layout(cont, LV_LAYOUT_GRID);

    uint32_t i;
    for (i = 0; i < 64; i++)
    {
        uint8_t col = i % 8;
        uint8_t row = i / 8;

        auto btn = new Button(col, row);
        btn->init(btn_event_cb, cont);
        btn->style(&style_btn2);
        btn->position(col, row, "");
        auto adr = btn->addr();
        buttons_map[adr] = btn;
    }
}
static const char *btn_label[2][8] = {
    {LV_SYMBOL_UP, LV_SYMBOL_DOWN, LV_SYMBOL_LEFT, LV_SYMBOL_RIGHT, "Session", "Drums", "Keys", "User"},
    {LV_SYMBOL_RIGHT, LV_SYMBOL_RIGHT, LV_SYMBOL_RIGHT, LV_SYMBOL_RIGHT, LV_SYMBOL_RIGHT, LV_SYMBOL_RIGHT, LV_SYMBOL_RIGHT, "Stop\nSolo\nMute"}};
static void lv_example_grid2(void)
{
    for (int n = 1; n < 2; n++)
    {
        uint8_t col = 10;

        for (int i = 0; i < 8; i++)
        {
            uint8_t row = i;

            auto btn = new Button(1, row, true);
            btn->init(btn_event_cb, cont);
            btn->style(&style_btn2);
            btn->position(col, row, btn_label[n][row]);
            auto adr = btn->addr();
            buttons_map[adr] = btn;
        }
    }

    {
        uint8_t col = 9;

        for (int i = 0; i < 4; i++)
        {
            uint8_t row = i;

            auto btn = new Button(0, row, true);
            btn->init(btn_event_cb, cont);
            btn->style(&style_btn2);
            btn->position(col, row, btn_label[0][row]);
            auto adr = btn->addr();
            buttons_map[adr] = btn;
        }
    }
}

static void init_lvgl_style()
{
    lv_style_init(&style_btn1);

    lv_style_set_bg_color(&style_btn1, lv_color_make(25, 15, 128));

    lv_style_init(&style_btn2);
    lv_style_set_bg_color(&style_btn2, lv_color_make(0, 128, 75));
}

static void _event_handler(void *event_handler_arg, esp_event_base_t event_base, int32_t event_id, void *arg)
{
    auto tpl = *(std::tuple<int, uint8_t *> *)arg;
    auto size = std::get<0>(tpl);
    auto data = std::get<1>(tpl);
    if (size > 512)
        return;
    for (size_t i = 0; i < size;)
    {
        auto cmd = data + i;
        uint8_t len = 4;

        int event_id = cmd[1] & 0xf0;
        switch (event_id)
        {
        case 0:
            return;
            break;
        case 0xf0:
            len = size;
            break;

        default:
        {
            midi_event_btn(cmd);
            break;
        }
        }

        i += len;
    }
    free(data);
}

void lvgl_open_midi()
{
    dawMode.setMIDI(midi);
    esp_event_handler_instance_register_with(usb::event_loop, USB_MIDI_BASE_EVENT, 0xffff, _event_handler, NULL, NULL);
}

void open_session()
{
    init_lvgl_style();

    lv_example_grid();
    lv_example_grid2();

    dawMode.init(btn_event_cb, cont);
    dawMode.style(&style_btn2);
    dawMode.position(11, 0, "DAW\nmode");

    fadersMode.init(btn_event_cb, cont);
    fadersMode.style(&style_btn2);
    fadersMode.position(11, 1, "Faders");

    session.init(btn_event_cb, cont);
    session.style(&style_btn2);
    session.position(9, 4, "Session");

    drums.init(btn_event_cb, cont);
    drums.style(&style_btn2);
    drums.position(9, 5, "Drums");

    keys.init(btn_event_cb, cont);
    keys.style(&style_btn2);
    keys.position(9, 6, "Keys");

    user.init(btn_event_cb, cont);
    user.style(&style_btn2);
    user.position(9, 7, "User");
}

static uint8_t fadLevel[] = {};
void Button::onPadPress()
{
    auto adr = addr();

    {
        style(&style_btn2, &style_btn1);
        auto c = 0x7f;
        uint8_t len = 0;
        if (!_cc)
            len = pad.padOn(buf, adr, c);
        else if (_col)
            len = pad.controlRight(buf, _row + 1, c);
        else
            len = pad.controlTop(buf, _row, c);

        midi->write(buf, len);
        _level = c;
    }
}

void Button::onRelease()
{

}

void midi_event_btn(void *arg)
{
    auto b0 = ((uint8_t *)arg)[0];
    auto b1 = ((uint8_t *)arg)[1];
    auto n = ((uint8_t *)arg)[2];
    auto lvl = ((uint8_t *)arg)[3];
    auto btn = buttons_map[n];

    switch (b0)
    {
    case 0x09:
    case 0x19:
    {
        if (btn)
        {
            btn->_level = lvl;
            btn->color(lvl);
        }

        break;
    }
    case 0x0b:
    {
        if (btn)
        {
            btn->color(lvl);
        }

        break;
    }

    default:
        break;
    }
}

/*-------------------------------------------------------------------*/
void DAWbutton::onPadPress()
{
    daw = !daw;
    if (daw)
        style(&style_btn2, &style_btn1);

    auto len = pad.dawMode(buf, daw);
    if (midi)
        midi->write(buf, len);
}

void DAWbutton::onRelease()
{
    if (!daw)
    {
        // _level = 0;
        style(&style_btn1, &style_btn2);
        Button::onRelease();
        fadersMode.changeMode();
    }
    changeMode();
}

void DAWbutton::changeMode()
{

}

void Faders::onPadPress()
{
    _level = _color;
    auto len = pad.fadersLayout(buf);
    midi->write(buf, len);
    style(&style_btn2, &style_btn1);
}

void Faders::onRelease()
{
    session.changeMode();
    drums.changeMode();
    keys.changeMode();
    user.changeMode();
}

void Faders::changeMode()
{
    style(&style_btn1, &style_btn2);
    _level = 0;
}

void Session::onPadPress()
{
    auto len = pad.controlTop(buf, 4, 0x7f);
    if (midi)
        midi->write(buf, len);
}

void Session::onRelease()
{
    fadersMode.changeMode();
    drums.changeMode();
    keys.changeMode();
    user.changeMode();
}

void Session::changeMode()
{
    style(&style_btn1, &style_btn2);
}

void Drums::onPadPress()
{
    auto len = pad.controlTop(buf, 5, 0x7f);
    if (midi)
        midi->write(buf, len);
}

void Drums::onRelease()
{
    session.changeMode();
    fadersMode.changeMode();
    keys.changeMode();
    user.changeMode();
}

void Drums::changeMode()
{
    style(&style_btn1, &style_btn2);
}

void Keys::onPadPress()
{
    auto len = pad.controlTop(buf, 6, 0x7f);
    if (midi)
        midi->write(buf, len);
}

void Keys::onRelease()
{
    session.changeMode();
    drums.changeMode();
    fadersMode.changeMode();
    user.changeMode();
}

void Keys::changeMode()
{
    style(&style_btn1, &style_btn2);
}

void User::onPadPress()
{
    auto len = pad.controlTop(buf, 7, 0x7f);
    if (midi)
        midi->write(buf, len);
}

void User::onRelease()
{
    session.changeMode();
    drums.changeMode();
    keys.changeMode();
    fadersMode.changeMode();
}

void User::changeMode()
{
    style(&style_btn1, &style_btn2);
}
