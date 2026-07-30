#pragma once

#include "container.h"
#include "button.h"

typedef struct UI_Context {
    Container *root;
    Button_Context *bc;
} UI_Context;