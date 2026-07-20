#pragma once

#include "button.h"

typedef struct UpdateContext {
    int previous_frame_time;
    float delta_time;
} UpdateContext;

UpdateContext* update_context_initialize(void);
void update_context_teardown(UpdateContext *p_uc);
void update(UpdateContext *p_uc, ButtonContext *p_bc);