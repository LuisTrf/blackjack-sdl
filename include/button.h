#pragma once

#include <SDL3/SDL.h>
#include <stdbool.h>
#include "constants.h"
#include "events.h"
#include "widget.h"
#include "game.h"

#define MOVE_BUTTON_WIDTH 168
#define MOVE_BUTTON_HEIGHT 70
#define MOVE_BUTTON_SEPARATION_X 20
#define MOVE_BUTTON_STEP_X (MOVE_BUTTON_WIDTH + MOVE_BUTTON_SEPARATION_X)
#define MOVE_BUTTON_X_ORIGIN (WINDOW_WIDTH/2.f - MOVE_BUTTON_WIDTH/2.f)
#define MOVE_BUTTON_Y_ORIGIN (WINDOW_HEIGHT - 160)

#define STACK_BUTTON_X_ORIGIN (WINDOW_WIDTH/2.f - CHIP_BUTTON_WIDTH/2.f)
#define STACK_BUTTON_Y_ORIGIN (WINDOW_HEIGHT/2.f - CHIP_BUTTON_HEIGHT*2.2f)

#define CHIP_BUTTON_WIDTH 122
#define CHIP_BUTTON_HEIGHT 134
#define CHIP_BUTTON_SEPARATION_X 10
#define CHIP_BUTTON_SEPARATION_Y 20
#define CHIP_BUTTON_STEP_X (CHIP_BUTTON_WIDTH + CHIP_BUTTON_SEPARATION_X)
#define CHIP_BUTTON_X_ORIGIN (WINDOW_WIDTH/2.f - CHIP_BUTTON_WIDTH/2.f - CHIP_BUTTON_STEP_X*2.f)
#define CHIP_BUTTON_Y_ORIGIN (WINDOW_HEIGHT/2.f - CHIP_BUTTON_HEIGHT*1.4f)

#define ABS(a) (((a) < 0) ? -(a) : (a))
#define CHIP_BUTTON_X(i) (CHIP_BUTTON_X_ORIGIN + (i%5)*(CHIP_BUTTON_WIDTH+CHIP_BUTTON_SEPARATION_X))
#define CHIP_BUTTON_Y(i) (CHIP_BUTTON_Y_ORIGIN + (i/5)*(CHIP_BUTTON_HEIGHT+CHIP_BUTTON_SEPARATION_Y) - 30*ABS(2-(i%5)) + 60)

typedef enum {
    _BUTTON_STATE_NONE,
    BUTTON_STATE_IDLE,
    BUTTON_STATE_DISABLED,
    BUTTON_STATE_HOVERED,
    BUTTON_STATE_PRESSED
} BUTTON_STATE;

typedef struct Button{
    Widget widget;
    Event release_event;
    BUTTON_STATE _state;
    BUTTON_STATE _prev_state;
    SDL_Texture *p_spritesheet;
} Button;

typedef struct ButtonContext {
    Button** move_button_arr;
} ButtonContext;

Button* button_create(
    float x, float y, int width, int height, 
    bool visible, 
    EventType release_eventtype, 
    BUTTON_STATE button_state_initial, 
    SDL_Texture* spritesheet,
    Event (*notify_func)(Widget *self, Event event),
    Event (*input_func)(Widget *self, SDL_Event event)
);
ButtonContext* button_context_create(void);
void button_destroy(Button *p_button);
void button_context_destroy(ButtonContext *p_bc);
void button_set_state(Button *button, BUTTON_STATE state);
BUTTON_STATE button_get_state(Button *button);
BUTTON_STATE button_get_prev_state(Button *button);
void button_restore_prev_state(Button *button);
void bc_register_move_button(ButtonContext *p_bc, Button *button);
int bc_get_visible_move_buttons(ButtonContext *p_bc);
void bc_update_move_button_positions_from_visibilities(ButtonContext *p_bc);

Event button_notify_deal(Widget *self, Event event);
Event button_notify_hit(Widget *self, Event event);
Event button_notify_stand(Widget *self, Event event);