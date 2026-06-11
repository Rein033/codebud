#pragma once

#include "apps/app.h"

// Builds the home screen with one tile per registered app.
void launcher_show();

// Called from loop() so the active app can update itself.
void launcher_update();
