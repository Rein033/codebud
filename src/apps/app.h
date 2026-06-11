#pragma once

#include <lvgl.h>

// Interface implemented by every launcher app.
struct App {
    const char *name;
    const char *icon;                   // LVGL symbol shown on the launcher tile
    void (*create)(lv_obj_t *parent);   // build the app's UI inside parent
    void (*update)();                   // called periodically while the app is open, or nullptr
    void (*destroy)();                  // cleanup before returning to launcher, or nullptr
};
