#pragma once

#include "container.h"
#include "../input/input_listener.h"
#include "../event/event_listener.h"
#include "../render/render_types.h"

Container* ui_root_initialize(InputListener **input_listeners, EventListener **event_listeners, font_hash* font_map);
void widgets_teardown(Container *root);