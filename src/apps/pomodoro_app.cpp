#include "pomodoro_app.h"

// 25-minute pomodoro countdown timer with start/pause/reset controls.

namespace {
constexpr uint32_t POMODORO_SECONDS = 25 * 60;

lv_obj_t *label = nullptr;
lv_timer_t *timer = nullptr;
uint32_t remaining = POMODORO_SECONDS;
bool running = false;

void update_label() {
    lv_label_set_text_fmt(label, "%02u:%02u", remaining / 60, remaining % 60);
}

void tick(lv_timer_t *) {
    if (!running) return;
    if (remaining > 0) {
        remaining--;
        update_label();
    }
    if (remaining == 0) {
        running = false;
        lv_label_set_text(label, "Done!");
    }
}

void start_cb(lv_event_t *) { running = true; }

void pause_cb(lv_event_t *) { running = false; }

void reset_cb(lv_event_t *) {
    running = false;
    remaining = POMODORO_SECONDS;
    update_label();
}

lv_obj_t *make_button(lv_obj_t *parent, const char *text, lv_event_cb_t cb) {
    lv_obj_t *btn = lv_btn_create(parent);
    lv_obj_t *btn_label = lv_label_create(btn);
    lv_label_set_text(btn_label, text);
    lv_obj_center(btn_label);
    lv_obj_add_event_cb(btn, cb, LV_EVENT_CLICKED, nullptr);
    return btn;
}

void create(lv_obj_t *parent) {
    remaining = POMODORO_SECONDS;
    running = false;

    label = lv_label_create(parent);
    lv_obj_set_style_text_font(label, &lv_font_montserrat_28, 0);
    lv_obj_align(label, LV_ALIGN_CENTER, 0, -30);
    update_label();

    lv_obj_t *row = lv_obj_create(parent);
    lv_obj_set_size(row, LV_PCT(100), 60);
    lv_obj_align(row, LV_ALIGN_CENTER, 0, 50);
    lv_obj_set_flex_flow(row, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(row, LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_bg_opa(row, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_width(row, 0, 0);

    make_button(row, "Start", start_cb);
    make_button(row, "Pause", pause_cb);
    make_button(row, "Reset", reset_cb);

    timer = lv_timer_create(tick, 1000, nullptr);
}

void destroy() {
    if (timer) {
        lv_timer_del(timer);
        timer = nullptr;
    }
    label = nullptr;
}
}  // namespace

App pomodoro_app = {
    .name = "Pomodoro",
    .icon = LV_SYMBOL_BELL,
    .create = create,
    .update = nullptr,
    .destroy = destroy,
};
