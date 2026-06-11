#include "sensor_app.h"

#include <Arduino.h>

#include "config.h"

// Reads an analog sensor (e.g. LDR or potentiometer) and shows it as a bar + value.

namespace {
lv_obj_t *bar = nullptr;
lv_obj_t *label = nullptr;
lv_timer_t *timer = nullptr;

void update(lv_timer_t *) {
    int value = analogRead(SENSOR_PIN);
    lv_bar_set_value(bar, value, LV_ANIM_ON);
    lv_label_set_text_fmt(label, "%d", value);
}

void create(lv_obj_t *parent) {
    label = lv_label_create(parent);
    lv_obj_set_style_text_font(label, &lv_font_montserrat_28, 0);
    lv_obj_align(label, LV_ALIGN_CENTER, 0, -40);

    bar = lv_bar_create(parent);
    lv_obj_set_size(bar, LV_PCT(80), 24);
    lv_obj_align(bar, LV_ALIGN_CENTER, 0, 30);
    lv_bar_set_range(bar, 0, 4095);

    pinMode(SENSOR_PIN, INPUT);
    update(nullptr);
    timer = lv_timer_create(update, 200, nullptr);
}

void destroy() {
    if (timer) {
        lv_timer_del(timer);
        timer = nullptr;
    }
    bar = nullptr;
    label = nullptr;
}
}  // namespace

App sensor_app = {
    .name = "Sensor",
    .icon = LV_SYMBOL_CHARGE,
    .create = create,
    .update = nullptr,
    .destroy = destroy,
};
