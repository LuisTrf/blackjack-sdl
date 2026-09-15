#pragma once

#include "../rect.h"

typedef enum WIDGET_TYPE {
    WIDGET_LABEL,
    WIDGET_BUTTON,
    WIDGET_PICBOX,
    WIDGET_CONTAINER,
    WIDGET_SPRITEBOX
} WIDGET_TYPE;

typedef struct Widget {
    Rect rect;
    WIDGET_TYPE wtype;
} Widget;