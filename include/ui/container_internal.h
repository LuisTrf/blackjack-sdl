#pragma once

#include "widget_internal.h"
#include "container.h"

typedef struct Container {
    Widget widget;
    Widget** children;
} Container;