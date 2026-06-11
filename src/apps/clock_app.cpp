#include "clock_app.h"

#include <Arduino.h>

// Simple uptime "clock" (HH:MM:SS since boot, no RTC required).

namespace {
lv_obj_t *label = nullptr;
lv_timer_t *timer = nullptr;

void update_label(lv_timer_t *) {
    uint32_t total_sec = millis() / 1000;
    uint32_t h = (total_sec / 3600) % 24;
    uint32_t m = (total_sec / 60) % 60;
    uint32_t s = total_sec % 60;
    lv_label_set_text_fmt(label, "%02u:%02u:%02u", h, m, s);
}

void create(lv_obj_t *parent) {
    label = lv_label_create(parent);
    lv_obj_set_style_text_font(label, &lv_font_montserrat_28, 0);
    lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);

    update_label(nullptr);
    timer = lv_timer_create(update_label, 1000, nullptr);
}

void destroy() {
    if (timer) {
        lv_timer_del(timer);
        timer = nullptr;
    }
    label = nullptr;
}
}  // namespace

App clock_app = {
    .name = "Clock",
    .icon = LV_SYMBOL_REFRESH,
    .create = create,
    .update = nullptr,
    .destroy = destroy,
};
