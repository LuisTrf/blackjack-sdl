#pragma once

#include <SDL3/SDL.h>
#include <stdbool.h>
#include "constants.h"
#include "events.h"
#include "widget.h"

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
    BUTTON_TYPE_ACTION,
    BUTTON_TYPE_GENERIC,
} BUTTON_TYPE;

typedef enum {
    _BUTTON_STATE_NONE,
    BUTTON_STATE_IDLE,
    BUTTON_STATE_HOVERED,
    BUTTON_STATE_PRESSED,
    BUTTON_STATE_RELEASED,
    BUTTON_STATE_DISABLED
} BUTTON_STATE;

typedef struct Button{
    Widget widget;
    BUTTON_TYPE btype;
    BUTTON_STATE _state;
    BUTTON_STATE _prev_state;
    SDL_Texture *p_spritesheet;
    void (*callback)(struct Button *self);
    Widget** subscribers;
} Button;

typedef struct ButtonContext {
    Button** alive_action_buttons;
} ButtonContext;

Button* button_create(ButtonContext *p_bc,
    float x, float y, int width, int height, bool visible, 
    BUTTON_TYPE btype, BUTTON_STATE button_state_initial, SDL_Texture* spritesheet,
    void (*callback_func)(Button *self),
    void (*update_func)(Widget *self, App_Event event)
);
ButtonContext* button_context_initialize(void);
void button_destroy(Button *p_button);
void button_context_teardown(ButtonContext *p_bc);
void button_set_state(Button *button, BUTTON_STATE state);
BUTTON_STATE button_get_state(Button *button);
BUTTON_STATE button_get_prev_state(Button *button);
void button_restore_prev_state(Button *button);
void button_add_subscriber(Button *publisher, Widget* subscriber);
void button_notify_all(Button *publisher, App_Event event);
int button_context_get_visible_action_buttons(ButtonContext *p_bc);
void button_context_update_action_button_positions_from_visibilities(ButtonContext *p_bc);

void button_update_deal(Widget *self, App_Event event);
void button_update_hit(Widget *self, App_Event event);