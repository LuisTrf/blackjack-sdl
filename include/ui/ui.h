#pragma once

#include "container.h"
#include "button.h"
#include "../app_state.h"
#include "../render/render.h"
#include "../input/input_context.h"
#include "../event/event_context.h"

typedef struct UI_Context {
    Container *root;
    Button_Context *bc;
} UI_Context;

UI_Context* ui_context_create(texture_hash *texture_map, Input_Context *ic, Event_Context *ec);
void ui_context_destroy(UI_Context *uic);