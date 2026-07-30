#pragma once

#include "update.h"

typedef struct Update_Context {
    int previous_frame_time;
    float delta_time;
} Update_Context;