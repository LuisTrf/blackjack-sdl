#ifndef BUTTONS_H
#define BUTTONS_H

#include <SDL3/SDL.h>
#include <stdbool.h>
#include "game_constants.h"
#include "game_object.h"
#include "game.h"

#define BUTTON_WIDTH 168
#define BUTTON_HEIGHT 70
#define BUTTON_SEPARATION_X 20
#define BUTTON_STEP_X (BUTTON_WIDTH + BUTTON_SEPARATION_X)
#define BUTTON_X_ORIGIN (WINDOW_WIDTH/2.f - BUTTON_WIDTH/2.f)
#define BUTTON_Y_ORIGIN (WINDOW_HEIGHT - 160)

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

#define STACK_BUTTON_X_ORIGIN (WINDOW_WIDTH/2.f - CHIP_BUTTON_WIDTH/2.f)
#define STACK_BUTTON_Y_ORIGIN (WINDOW_HEIGHT/2.f - CHIP_BUTTON_HEIGHT*2.2f)

#define NUMBER_OF_BUTTONS 17
#define NUMBER_OF_ACTION_BUTTONS 6
#define NUMBER_OF_VALUED_BUTTONS 10

typedef enum {
    _NONE_BUTTON_STATE,
    IDLE,
    HOVERED,
    PRESSED,
    RELEASED,
    DISABLED
} BUTTON_STATE;

typedef struct Button{
    GameObject obj;
    BUTTON_STATE _state;
    BUTTON_STATE _prev_state;
    SDL_Texture *spritesheet;
    void (*action)(struct Button *self);
} Button;

typedef struct ValuedButton{
    Button button;
    CHIP_VALUE val;
} ValuedButton;

void set_state(Button *button, BUTTON_STATE state);
BUTTON_STATE get_state(Button *button);
BUTTON_STATE get_prev_state(Button *button);
void restore_prev_state(Button *button);
void disable_all_buttons(void);
void hide_and_disable_all_action_buttons(void);
void hide_and_disable_all_valued_buttons(void);
void restore_all_button_prev_states(void);
int get_visible_action_buttons(void);
ValuedButton* get_valued_button_associated_to_value(CHIP_VALUE val);

extern Button deal_button;
extern Button bet_button;
extern Button hit_button;
extern Button stand_button;
extern Button insurance_button;
extern Button double_down_button;

extern Button* action_buttons[NUMBER_OF_ACTION_BUTTONS];

extern Button stack_button;

extern ValuedButton white1_button;
extern ValuedButton red5_button;
extern ValuedButton blue10_button;
extern ValuedButton green25_button;
extern ValuedButton black100_button;
extern ValuedButton purple500_button;
extern ValuedButton yellow1k_button;
extern ValuedButton orange5k_button;
extern ValuedButton redblue25k_button;
extern ValuedButton gold100k_button;

extern ValuedButton* valued_buttons[NUMBER_OF_VALUED_BUTTONS];

extern Button* buttons[NUMBER_OF_BUTTONS];

#endif