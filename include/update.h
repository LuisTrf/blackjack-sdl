#pragma once

#include "button.h"
#include "event_context.h"

typedef struct UpdateContext {
    int previous_frame_time;
    float delta_time;
} UpdateContext;

UpdateContext* update_context_create(void);
void update_context_destroy(UpdateContext *p_uc);
void update(UpdateContext *p_uc, GameContext *p_gc, EventContext *p_ec, ButtonContext *p_bc);