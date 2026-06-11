#pragma once

#include "apps/app.h"

// Builds the home screen with one tile per registered app.
void launcher_show();

// Called from loop() so the active app can update itself.
void launcher_update();

// Opens an app by its display name (case-insensitive). No-op if not found.
void launcher_open_app_by_name(const char *name);
