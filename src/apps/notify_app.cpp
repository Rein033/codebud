#include "notify_app.h"

#include <Arduino.h>

// Shows the most recent notification pushed over BLE.

namespace {
lv_obj_t *title_label = nullptr;
lv_obj_t *message_label = nullptr;

String stored_title = "No messages yet";
String stored_message = "";

void create(lv_obj_t *parent) {
    title_label = lv_label_create(parent);
    lv_obj_set_style_text_font(title_label, &lv_font_montserrat_28, 0);
    lv_label_set_long_mode(title_label, LV_LABEL_LONG_WRAP);
    lv_obj_set_width(title_label, LV_PCT(100));
    lv_label_set_text(title_label, stored_title.c_str());
    lv_obj_align(title_label, LV_ALIGN_TOP_MID, 0, 10);

    message_label = lv_label_create(parent);
    lv_label_set_long_mode(message_label, LV_LABEL_LONG_WRAP);
    lv_obj_set_width(message_label, LV_PCT(100));
    lv_label_set_text(message_label, stored_message.c_str());
    lv_obj_align(message_label, LV_ALIGN_TOP_MID, 0, 60);
}

void destroy() {
    title_label = nullptr;
    message_label = nullptr;
}
}  // namespace

App notify_app = {
    .name = "Messages",
    .icon = LV_SYMBOL_ENVELOPE,
    .create = create,
    .update = nullptr,
    .destroy = destroy,
};

void notify_app_set_message(const char *title, const char *message) {
    stored_title = title;
    stored_message = message;
    if (title_label) lv_label_set_text(title_label, stored_title.c_str());
    if (message_label) lv_label_set_text(message_label, stored_message.c_str());
}
