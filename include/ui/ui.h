#pragma once

#include "container.h"
#include "../input/input_context.h"
#include "../event/event_context.h"
#include "../render/render_types.h"

Container* ui_root_initialize(InputContext *input_ctx, EventContext *event_ctx, font_hash* font_map);
void widgets_teardown(Container *root);