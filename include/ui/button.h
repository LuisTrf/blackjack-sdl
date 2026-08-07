#pragma once

#include <SDL3/SDL.h>
#include <stdbool.h>
#include "widget.h"
#include "../event/event_types.h"
#include "../render/render_types.h"

typedef enum BUTTON_STATE{
    _BUTTON_STATE_NONE,
    BUTTON_STATE_IDLE,
    BUTTON_STATE_DISABLED,
    BUTTON_STATE_HOVERED,
    BUTTON_STATE_PRESSED
} BUTTON_STATE;

typedef struct Button {
    Widget widget;
    Event release_event;
    BUTTON_STATE _state;
    BUTTON_STATE _prev_state;
    TEXTURE_ID tid;
} Button;

Button* button_create(
    float x, float y, 
    int width, int height, 
    bool visible, 
    EventType release_eventtype, 
    BUTTON_STATE button_state_initial, 
    TEXTURE_ID tid
);
void button_destroy(Button *button);

void button_set_state(Button *button, BUTTON_STATE state);
BUTTON_STATE button_get_state(Button *button);
BUTTON_STATE button_get_prev_state(Button *button);
void button_restore_prev_state(Button *button);

void reposition_visible_move_buttons(Widget** moveb_widgets);

void button_notify_deal(void *self, Event event, void *dependencies);
void button_notify_hit(void *self, Event event, void *dependencies);
void button_notify_stand(void *self, Event event, void *dependencies);