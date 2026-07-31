#pragma once

#include "../app_state.h"

typedef struct UpdateContext {
    int previous_frame_time;
    float delta_time;
} UpdateContext;

UpdateContext* update_context_create(void);
void update_context_destroy(UpdateContext *update_ctx);
void update(AppState *as);