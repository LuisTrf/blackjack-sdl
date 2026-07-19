#include <SDL3/SDL.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../include/widget.h"
#include "../include/button.h"
#include "../include/stb_ds.h"

/*
Button deal_button = {{BUTTON_X_ORIGIN, BUTTON_Y_ORIGIN, BUTTON_WIDTH, BUTTON_HEIGHT, true},
    IDLE, _NONE_BUTTON_STATE, NULL, on_deal_released};
Button bet_button = {{BUTTON_X_ORIGIN, BUTTON_Y_ORIGIN, BUTTON_WIDTH, BUTTON_HEIGHT, true},
    IDLE, _NONE_BUTTON_STATE, NULL, on_bet_released};
Button hit_button = {{BUTTON_X_ORIGIN, BUTTON_Y_ORIGIN, BUTTON_WIDTH, BUTTON_HEIGHT, false},
    DISABLED, _NONE_BUTTON_STATE, NULL, on_hit_released};
Button stand_button = {{BUTTON_X_ORIGIN, BUTTON_Y_ORIGIN, BUTTON_WIDTH, BUTTON_HEIGHT, false},
    DISABLED, _NONE_BUTTON_STATE, NULL, on_stand_released};
Button insurance_button = {{BUTTON_X_ORIGIN, BUTTON_Y_ORIGIN, BUTTON_WIDTH, BUTTON_HEIGHT, false},
    DISABLED, _NONE_BUTTON_STATE, NULL, on_insurance_released};
Button double_down_button = {{BUTTON_X_ORIGIN, BUTTON_Y_ORIGIN, BUTTON_WIDTH, BUTTON_HEIGHT, false},
    DISABLED, _NONE_BUTTON_STATE, NULL, on_double_down_released};
Button stack_button = {{STACK_BUTTON_X_ORIGIN, STACK_BUTTON_Y_ORIGIN, CHIP_BUTTON_WIDTH, CHIP_BUTTON_HEIGHT, false},
    DISABLED, _NONE_BUTTON_STATE, NULL, on_stack_released};
Button white1_button = {{CHIP_BUTTON_X(0), CHIP_BUTTON_Y(0), CHIP_BUTTON_WIDTH, CHIP_BUTTON_HEIGHT, false},
    DISABLED, _NONE_BUTTON_STATE, NULL, on_valued_released};
Button red5_button = {{CHIP_BUTTON_X(1), CHIP_BUTTON_Y(1), CHIP_BUTTON_WIDTH, CHIP_BUTTON_HEIGHT, false},
    DISABLED, _NONE_BUTTON_STATE, NULL, on_valued_released};
Button blue10_button = {{{CHIP_BUTTON_X(2), CHIP_BUTTON_Y(2), CHIP_BUTTON_WIDTH, CHIP_BUTTON_HEIGHT, false},
    DISABLED, _NONE_BUTTON_STATE, NULL, on_valued_released}, TEN};
ValuedButton green25_button = {{{CHIP_BUTTON_X(3), CHIP_BUTTON_Y(3), CHIP_BUTTON_WIDTH, CHIP_BUTTON_HEIGHT, false},
    DISABLED, _NONE_BUTTON_STATE, NULL, on_valued_released}, TWENTY_FIVE};
ValuedButton black100_button = {{{CHIP_BUTTON_X(4), CHIP_BUTTON_Y(4), CHIP_BUTTON_WIDTH, CHIP_BUTTON_HEIGHT, false},
    DISABLED, _NONE_BUTTON_STATE, NULL, on_valued_released}, HUNDRED};
ValuedButton purple500_button = {{{CHIP_BUTTON_X(5), CHIP_BUTTON_Y(5), CHIP_BUTTON_WIDTH, CHIP_BUTTON_HEIGHT, false},
    DISABLED, _NONE_BUTTON_STATE, NULL, on_valued_released}, FIVE_HUNDRED};
ValuedButton yellow1k_button = {{{CHIP_BUTTON_X(6), CHIP_BUTTON_Y(6), CHIP_BUTTON_WIDTH, CHIP_BUTTON_HEIGHT, false},
    DISABLED, _NONE_BUTTON_STATE, NULL, on_valued_released}, ONE_K};
ValuedButton orange5k_button = {{{CHIP_BUTTON_X(7), CHIP_BUTTON_Y(7), CHIP_BUTTON_WIDTH, CHIP_BUTTON_HEIGHT, false},
    DISABLED, _NONE_BUTTON_STATE, NULL, on_valued_released}, FIVE_K};
Button redblue25k_button = {{{CHIP_BUTTON_X(8), CHIP_BUTTON_Y(8), CHIP_BUTTON_WIDTH, CHIP_BUTTON_HEIGHT, false},
    DISABLED, _NONE_BUTTON_STATE, NULL, on_valued_released}, TWENTY_FIVE_K};
Button gold100k_button = {{{CHIP_BUTTON_X(9), CHIP_BUTTON_Y(9), CHIP_BUTTON_WIDTH, CHIP_BUTTON_HEIGHT, false},
    DISABLED, _NONE_BUTTON_STATE, NULL, on_valued_released}, HUNDRED_K};
*/

Button* button_create(float x, float y, int width, int height, bool visible, BUTTON_STATE button_state_initial, SDL_Texture* spritesheet, void (*callback_func)(Button *self),
    void (*update_func)(Widget *self, App_Event event)
)
{
    Button button = {
        .widget={.wtype=WIDGET_BUTTON, .pos={.x=x, .y=y}, .width=width, .height=height, .visible=visible, .update_func=update_func}, 
        ._state=button_state_initial, ._prev_state=_BUTTON_STATE_NONE, .p_spritesheet=spritesheet, .callback=callback_func, .subscribers=NULL
    };
    Button *p_button = malloc(sizeof(Button));
    if (p_button == NULL){
        abort();
    }
    *p_button = button;
    return p_button;
}

void button_destroy(Button *p_button){
    arrfree(p_button->subscribers);
    p_button->subscribers = NULL;
    free(p_button);
    p_button = NULL;
}

void button_set_state(Button *button, BUTTON_STATE state){
    button->_prev_state = button->_state;
    button->_state = state;
}

BUTTON_STATE button_get_state(Button *button){
    return button->_state;
}

BUTTON_STATE button_get_prev_state(Button *button){
    return button->_prev_state;
}

void button_restore_prev_state(Button *button){
    BUTTON_STATE temp = button->_state;
    button->_state = button->_prev_state;
    button->_prev_state = temp;
}

void button_add_subscriber(Button *publisher, Widget* subscriber){
    arrput(publisher->subscribers, subscriber);
}

void button_notify_all(Button *publisher, App_Event event){
    for (int i = 0; i < arrlen(publisher->subscribers); i++){
        publisher->subscribers[i]->update_func(publisher->subscribers[i], event);
    }
}