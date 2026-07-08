#include "../include/buttons.h"
#include "../include/button_events.h"

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
Button* action_buttons[NUMBER_OF_ACTION_BUTTONS] = {
    &deal_button,
    &bet_button,
    &hit_button,
    &stand_button,
    &insurance_button,
    &double_down_button,
};
Button stack_button = {{STACK_BUTTON_X_ORIGIN, STACK_BUTTON_Y_ORIGIN, CHIP_BUTTON_WIDTH, CHIP_BUTTON_HEIGHT, false},
    DISABLED, _NONE_BUTTON_STATE, NULL, on_stack_released};
ValuedButton white1_button = {{{CHIP_BUTTON_X(0), CHIP_BUTTON_Y(0), CHIP_BUTTON_WIDTH, CHIP_BUTTON_HEIGHT, false},
    DISABLED, _NONE_BUTTON_STATE, NULL, on_valued_released}, ONE};
ValuedButton red5_button = {{{CHIP_BUTTON_X(1), CHIP_BUTTON_Y(1), CHIP_BUTTON_WIDTH, CHIP_BUTTON_HEIGHT, false},
    DISABLED, _NONE_BUTTON_STATE, NULL, on_valued_released}, FIVE};
ValuedButton blue10_button = {{{CHIP_BUTTON_X(2), CHIP_BUTTON_Y(2), CHIP_BUTTON_WIDTH, CHIP_BUTTON_HEIGHT, false},
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
ValuedButton redblue25k_button = {{{CHIP_BUTTON_X(8), CHIP_BUTTON_Y(8), CHIP_BUTTON_WIDTH, CHIP_BUTTON_HEIGHT, false},
    DISABLED, _NONE_BUTTON_STATE, NULL, on_valued_released}, TWENTY_FIVE_K};
ValuedButton gold100k_button = {{{CHIP_BUTTON_X(9), CHIP_BUTTON_Y(9), CHIP_BUTTON_WIDTH, CHIP_BUTTON_HEIGHT, false},
    DISABLED, _NONE_BUTTON_STATE, NULL, on_valued_released}, HUNDRED_K};
ValuedButton* valued_buttons[NUMBER_OF_VALUED_BUTTONS] = {
    &white1_button,
    &red5_button,
    &blue10_button,
    &green25_button,
    &black100_button,
    &purple500_button,
    &yellow1k_button,
    &orange5k_button,
    &redblue25k_button,
    &gold100k_button
};
Button* buttons[NUMBER_OF_BUTTONS] = {
    &deal_button,
    &bet_button,
    &hit_button,
    &stand_button,
    &insurance_button,
    &double_down_button,
    &stack_button,
    &(white1_button.button),
    &(red5_button.button),
    &(blue10_button.button),
    &(green25_button.button),
    &(black100_button.button),
    &(purple500_button.button),
    &(yellow1k_button.button),
    &(orange5k_button.button),
    &(redblue25k_button.button),
    &(gold100k_button.button)
};

void set_state(Button *button, BUTTON_STATE state){
    button->_prev_state=button->_state;
    button->_state=state;
}

BUTTON_STATE get_state(Button *button){
    return button->_state;
}

BUTTON_STATE get_prev_state(Button *button){
    return button->_prev_state;
}

void restore_prev_state(Button *button){
    BUTTON_STATE temp = button->_state;
    button->_state=button->_prev_state;
    button->_prev_state=temp;
}

void disable_all_action_buttons(void){
    for (int i=0; i<NUMBER_OF_ACTION_BUTTONS; i++){
        set_state(action_buttons[i], DISABLED);
    }
}

void disable_all_valued_buttons(void){
    for (int i=0; i<NUMBER_OF_VALUED_BUTTONS; i++){
        set_state(&(valued_buttons[i]->button), DISABLED);
    }
}

void disable_all_buttons(void){
    for (int i=0; i<NUMBER_OF_BUTTONS; i++){
        set_state(buttons[i], DISABLED);
    }
}

void hide_and_disable_all_valued_buttons(void){
    for (int i=0; i<NUMBER_OF_VALUED_BUTTONS; i++){
        set_state(&(valued_buttons[i]->button), DISABLED);
        valued_buttons[i]->button.obj.visible = false;
    }
}

void hide_and_disable_all_action_buttons(void){
    for (int i=0; i<NUMBER_OF_ACTION_BUTTONS; i++){
        set_state(action_buttons[i], DISABLED);
        action_buttons[i]->obj.visible=false;
    }
}

void restore_all_button_prev_states(void){
    for (int i=0; i<NUMBER_OF_BUTTONS; i++){
        restore_prev_state(buttons[i]);
    }
}

int get_visible_action_buttons(void){
    int visible_action_buttons = 0;
    for (int i=0; i<NUMBER_OF_ACTION_BUTTONS; i++){
        if (action_buttons[i]->obj.visible){
            visible_action_buttons++;
        }
    }
    return visible_action_buttons;
}

ValuedButton* get_valued_button_associated_to_value(CHIP_VALUE val){
    for (int i=0; i<NUMBER_OF_VALUED_BUTTONS; i++){
        if (valued_buttons[i]->val==val){
            return valued_buttons[i];
        }
    }
    return NULL;
}