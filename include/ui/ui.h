#pragma once

#include "container.h"
#include "../input/input_listener.h"
#include "../event/event_listener.h"
#include "../render/render_types.h"
#include "../update/animation.h"

Container* ui_root_initialize(AppState *as);
void widgets_teardown(Container *root);