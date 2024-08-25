
#include "launchpad-mini-mk3.h"

#include "button.h"
extern novation::Launchpad pad;
usb::MIDI* Button::midi = nullptr;

void Button::init(lv_event_cb_t event_cb, lv_obj_t* obj)
{
    btn = lv_button_create(obj);       /*Add a button the current screen*/
    // lv_obj_set_pos(btn, 10, 10);                                /*Set its position*/
    lv_obj_set_size(btn, 72, 72);                               /*Set its size*/
    lv_obj_add_event_cb(btn, event_cb, LV_EVENT_ALL, this); /*Assign a callback to the button*/
    grid = obj;
}

void Button::style(lv_style_t *style)
{
    lv_obj_add_style(btn, style, LV_PART_MAIN | LV_STATE_DEFAULT);
}

void Button::style(lv_style_t *old_style, lv_style_t* new_style)
{
    lv_obj_replace_style(btn, old_style, new_style, LV_PART_MAIN | LV_STATE_DEFAULT);
}


