#pragma once

#include <SDL3/SDL.h>
#include <stdbool.h>
#include "widget.h"
#include "../event/event_types.h"

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
    SDL_Texture *p_spritesheet;
} Button;

typedef struct Button_Context{
    Button** moveb_refs;
} Button_Context;

Button* button_create(
    float x, float y, 
    int width, int height, 
    bool visible, 
    EventType release_eventtype, 
    BUTTON_STATE button_state_initial, 
    SDL_Texture* spritesheet,
    Event (*notify_func)(Widget *self, Event event),
    Event (*input_func)(Widget *self, SDL_Event event)
);
void button_destroy(Button *p_button);

Button_Context* button_context_create(void);
void button_context_destroy(Button_Context *p_bc);

void button_set_state(Button *button, BUTTON_STATE state);
BUTTON_STATE button_get_state(Button *button);
BUTTON_STATE button_get_prev_state(Button *button);
void button_restore_prev_state(Button *button);

void bc_register_move_button(Button_Context *p_bc, Button *button);
int bc_get_visible_move_buttons(Button_Context *p_bc);
void bc_update_move_button_positions_from_visibilities(Button_Context *p_bc);

Event button_notify_deal(Widget *self, Event event);
Event button_notify_hit(Widget *self, Event event);
Event button_notify_stand(Widget *self, Event event);