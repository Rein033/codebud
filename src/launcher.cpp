#include "launcher.h"

#include <strings.h>

#include "apps/clock_app.h"
#include "apps/notify_app.h"
#include "apps/pomodoro_app.h"
#include "apps/pong_app.h"
#include "apps/sensor_app.h"
#include "ble_service.h"

namespace {
App *apps[] = {&clock_app, &pomodoro_app, &pong_app, &sensor_app, &notify_app};
constexpr size_t NUM_APPS = sizeof(apps) / sizeof(apps[0]);

App *active_app = nullptr;

void open_app(App *app) {
    active_app = app;

    lv_obj_t *scr = lv_obj_create(nullptr);
    lv_obj_set_flex_flow(scr, LV_FLEX_FLOW_COLUMN);
    lv_obj_clear_flag(scr, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_t *top = lv_obj_create(scr);
    lv_obj_set_size(top, LV_PCT(100), 36);
    lv_obj_set_style_border_width(top, 0, 0);
    lv_obj_set_flex_flow(top, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(top, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    lv_obj_t *back_btn = lv_btn_create(top);
    lv_obj_t *back_label = lv_label_create(back_btn);
    lv_label_set_text(back_label, LV_SYMBOL_LEFT " Back");
    lv_obj_add_event_cb(back_btn, [](lv_event_t *) {
        if (active_app && active_app->destroy) active_app->destroy();
        active_app = nullptr;
        launcher_show();
    }, LV_EVENT_CLICKED, nullptr);

    lv_obj_t *title = lv_label_create(top);
    lv_label_set_text(title, app->name);
    lv_obj_set_style_pad_left(title, 12, 0);

    lv_obj_t *content = lv_obj_create(scr);
    lv_obj_set_width(content, LV_PCT(100));
    lv_obj_set_flex_grow(content, 1);
    lv_obj_set_style_border_width(content, 0, 0);
    lv_obj_set_style_pad_all(content, 4, 0);

    app->create(content);

    lv_scr_load(scr);
    ble_set_status(app->name);
}

void tile_cb(lv_event_t *e) {
    open_app(static_cast<App *>(lv_event_get_user_data(e)));
}
}  // namespace

void launcher_show() {
    lv_obj_t *scr = lv_obj_create(nullptr);
    lv_obj_set_flex_flow(scr, LV_FLEX_FLOW_ROW_WRAP);
    lv_obj_set_flex_align(scr, LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    for (size_t i = 0; i < NUM_APPS; i++) {
        App *app = apps[i];

        lv_obj_t *tile = lv_btn_create(scr);
        lv_obj_set_size(tile, 100, 100);
        lv_obj_set_flex_flow(tile, LV_FLEX_FLOW_COLUMN);
        lv_obj_set_flex_align(tile, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
        lv_obj_add_event_cb(tile, tile_cb, LV_EVENT_CLICKED, app);

        lv_obj_t *icon = lv_label_create(tile);
        lv_label_set_text(icon, app->icon);
        lv_obj_set_style_text_font(icon, &lv_font_montserrat_28, 0);

        lv_obj_t *label = lv_label_create(tile);
        lv_label_set_text(label, app->name);
    }

    lv_scr_load(scr);
    ble_set_status("home");
}

void launcher_update() {
    if (active_app && active_app->update) active_app->update();
}

void launcher_open_app_by_name(const char *name) {
    for (size_t i = 0; i < NUM_APPS; i++) {
        if (strcasecmp(apps[i]->name, name) == 0) {
            if (active_app && active_app->destroy) active_app->destroy();
            open_app(apps[i]);
            return;
        }
    }
}
