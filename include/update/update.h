#pragma once

#include "../app_state.h"

typedef struct Update_Context {
    int previous_frame_time;
    float delta_time;
} Update_Context;

Update_Context* update_context_create(void);
void update_context_destroy(Update_Context *p_uc);
void update(App_State *as);