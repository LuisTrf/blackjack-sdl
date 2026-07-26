#include <SDL3/SDL.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "../include/widget.h"
#include "../include/button.h"
#include "../include/stb_ds.h"
#include "../include/events.h"

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

Button* button_create(
    float x, float y, int width, int height, 
    bool visible, 
    EventType release_eventtype,
    BUTTON_STATE button_state_initial, 
    SDL_Texture* spritesheet, 
    Event (*notify_func)(Widget *self, Event event),
    Event (*input_func)(Widget *self, SDL_Event event)
)
{
    Button button = {
        .widget={
            .wtype=WIDGET_BUTTON, 
            .pos={.x=x, .y=y}, 
            .width=width, 
            .height=height, 
            .visible=visible, 
            .notify_func=notify_func,
            .input_func=input_func
        }, 
        .release_event=NULL_EVENT, 
        ._state=button_state_initial, 
        ._prev_state=_BUTTON_STATE_NONE, 
        .p_spritesheet=spritesheet, 
    };
    Button *p_button = malloc(sizeof(Button));
    if (p_button == NULL){
        abort();
    }
    Event release_event = button_event_create(release_eventtype, p_button);
    button.release_event = release_event;
    *p_button = button;
    return p_button;
}

ButtonContext* button_context_create(void){
    ButtonContext bc = {NULL};
    ButtonContext *p_bc = malloc(sizeof(ButtonContext));
    if (p_bc == NULL){
        abort();
    }
    *p_bc = bc;
    return p_bc;
}

void button_destroy(Button *p_button){
    free(p_button);
    p_button = NULL;
}

void button_context_destroy(ButtonContext *p_bc){
    arrfree(p_bc->move_button_arr);
    p_bc->move_button_arr = NULL;
    free(p_bc);
}

void bc_register_move_button(ButtonContext *p_bc, Button *button){
    arrput(p_bc->move_button_arr, button);
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

int bc_get_visible_move_buttons(ButtonContext *p_bc){
    int visible_move_button_count = 0;
    for (int i = 0; i < arrlen(p_bc->move_button_arr); i++){
        if (p_bc->move_button_arr[i]->widget.visible){
            visible_move_button_count++;
        }
    }
    return visible_move_button_count;
}

void bc_update_move_button_positions_from_visibilities(ButtonContext *p_bc){
    int visible_move_button_count = bc_get_visible_move_buttons(p_bc);
    int remaining_visible_move_button_count = visible_move_button_count;
    for (int i = 0; i < arrlen(p_bc->move_button_arr); i++){
        if (p_bc->move_button_arr[i]->widget.visible){
            p_bc->move_button_arr[i]->widget.pos.x = 
                MOVE_BUTTON_X_ORIGIN
                + 0.5f * (visible_move_button_count-1) * MOVE_BUTTON_WIDTH
                - 0.5f * (remaining_visible_move_button_count-1) * (MOVE_BUTTON_STEP_X + MOVE_BUTTON_WIDTH);
            remaining_visible_move_button_count--;
        }
        else {
            p_bc->move_button_arr[i]->widget.pos.x = MOVE_BUTTON_X_ORIGIN;
        }
    }
}

Event button_notify_deal(Widget *self, Event event){
    Event e;
    switch(event.type){
        case BUTTON_EVENT_RELEASE_HIT:
            printf("i, deal, called from hit release!\n");
            break;
        default:
            break;
    }
    return NULL_EVENT;
}

Event button_notify_hit(Widget *self, Event event){
    Event e;
    switch(event.type){
        case BUTTON_EVENT_RELEASE_DEAL:
            printf("i, hit, called from deal release!\n");
            break;
        default:
            break;
    }
    return NULL_EVENT;
}