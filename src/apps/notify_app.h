#pragma once

#include "app.h"

extern App notify_app;

// Updates the displayed notification, including while the app is open.
void notify_app_set_message(const char *title, const char *message);
