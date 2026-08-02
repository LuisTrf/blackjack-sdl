#pragma once

#include "container.h"
#include "button_context.h"
#include "../app_state.h"
#include "../render/render_types.h"
#include "../input/input_context.h"
#include "../event/event_context.h"

typedef struct UIContext {
    Container *root;
    ButtonContext *button_ctx;
} UIContext;

UIContext* ui_context_create(font_hash* font_map, texture_hash *texture_map, InputContext *input_ctx, EventContext *event_ctx);
void ui_context_destroy(UIContext *ui_ctx);