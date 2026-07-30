#pragma once

#include "button.h"
#include "widget_internal.h"

typedef struct Button {
    Widget widget;
    Event release_event;
    BUTTON_STATE _state;
    BUTTON_STATE _prev_state;
    SDL_Texture *p_spritesheet;
} Button;

typedef struct Button_Context{
    Button** moveb_refs;
} Button_Context;