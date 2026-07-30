#pragma once

#include "../main.h"

typedef struct Update_Context Update_Context;

Update_Context* update_context_create(void);
void update_context_destroy(Update_Context *p_uc);
void update(App_State *as);