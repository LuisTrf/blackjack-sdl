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

typedef struct ButtonContext{
    Button** moveb_refs;
} ButtonContext;

Button* button_create(
    float x, float y, 
    int width, int height, 
    bool visible, 
    EventType release_eventtype, 
    BUTTON_STATE button_state_initial, 
    TEXTURE_ID tid,
    Event (*notify_func)(Widget *self, Event event),
    Event (*input_func)(Widget *self, SDL_Event sdl_event)
);
void button_destroy(Button *button);

ButtonContext* button_context_create(void);
void button_context_destroy(ButtonContext *button_ctx);

void button_set_state(Button *button, BUTTON_STATE state);
BUTTON_STATE button_get_state(Button *button);
BUTTON_STATE button_get_prev_state(Button *button);
void button_restore_prev_state(Button *button);

void button_ctx_register_move_button(ButtonContext *button_ctx, Button *button);
int button_ctx_get_visible_move_buttons(ButtonContext *button_ctx);
void button_ctx_reposition_visible_move_buttons(ButtonContext *button_ctx);

Event button_notify_deal(Widget *self, Event event);
Event button_notify_hit(Widget *self, Event event);
Event button_notify_stand(Widget *self, Event event);