#pragma once

#include <SDL3/SDL.h>
#include <stdbool.h>
#include "constants.h"
#include "events.h"
#include "widget.h"
#include "game.h"

#define ACTION_BUTTON_WIDTH 168
#define ACTION_BUTTON_HEIGHT 70
#define ACTION_BUTTON_SEPARATION_X 20
#define ACTION_BUTTON_STEP_X (ACTION_BUTTON_WIDTH + ACTION_BUTTON_SEPARATION_X)
#define ACTION_BUTTON_X_ORIGIN (WINDOW_WIDTH/2.f - ACTION_BUTTON_WIDTH/2.f)
#define ACTION_BUTTON_Y_ORIGIN (WINDOW_HEIGHT - 160)

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
    Button** dynamically_positioned_buttons;
} ButtonContext;

Button* button_create(
    float x, float y, int width, int height, 
    bool visible, 
    App_EventType release_eventtype, 
    BUTTON_STATE button_state_initial, 
    SDL_Texture* spritesheet,
    Event (*notify_func)(Widget *self, Event event)
);
ButtonContext* button_context_initialize(void);
void button_destroy(Button *p_button);
void button_context_teardown(ButtonContext *p_bc);
void button_set_state(Button *button, BUTTON_STATE state);
BUTTON_STATE button_get_state(Button *button);
BUTTON_STATE button_get_prev_state(Button *button);
void button_restore_prev_state(Button *button);
void button_context_add_dynamically_positioned_button(ButtonContext *p_bc, Button *button);
int button_context_get_visible_dynamically_positioned_buttons(ButtonContext *p_bc);

Event button_notify_deal(Widget *self, Event event);
Event button_notify_hit(Widget *self, Event event);

void button_callback_deal(GameContext *gc, Button *self);
void button_callback_hit(GameContext *gc, Button *self);