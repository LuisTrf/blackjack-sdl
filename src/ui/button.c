#include <SDL3/SDL.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../../include/ui/button_constants.h"
#include "../../include/event/event_types.h"
#include "../../include/ui/button.h"
#include "../../include/stb_ds.h"
#include "../../include/game/game.h"

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
    float x, float y, 
    int width, int height, 
    bool visible, 
    EventType release_eventtype,
    BUTTON_STATE button_state_initial, 
    TEXTURE_ID tid
)
{
    Button button = {
        .widget={
            .rect = {
                .pos={.x=x, .y=y}, 
                .width=width, 
                .height=height, 
                .visible=visible, 
            },
            .wtype=WIDGET_BUTTON,
        }, 
        .release_event=(Event){.input={.type=release_eventtype}}, 
        ._state=button_state_initial, 
        ._prev_state=_BUTTON_STATE_NONE, 
        .tid=tid, 
    };
    Button *p_button = malloc(sizeof(Button));
    if (p_button == NULL){
        abort();
    }
    *p_button = button;
    return p_button;
}

void button_destroy(Button *p_button){
    free(p_button);
    p_button = NULL;
}

Event button_get_release_event(Button *button){
    return button->release_event;
}

BUTTON_STATE button_get_state(Button *button){
    return button->_state;
}

void button_set_state(Button *button, BUTTON_STATE state){
    button->_prev_state = button->_state;
    button->_state = state;
}

BUTTON_STATE button_get_prev_state(Button *button){
    return button->_prev_state;
}

void button_restore_prev_state(Button *button){
    BUTTON_STATE temp = button->_state;
    button->_state = button->_prev_state;
    button->_prev_state = temp;
}

void reposition_visible_move_buttons(Widget** moveb_widgets){
    int visible_move_button_count = 0;
    for (int i = 0; i < arrlen(moveb_widgets); i++){
        if (moveb_widgets[i]->rect.visible){
            visible_move_button_count++;
        }
    }
    int remaining_visible_move_button_count = visible_move_button_count;
    for (int i = 0; i < arrlen(moveb_widgets); i++){
        if (moveb_widgets[i]->rect.visible){
            moveb_widgets[i]->rect.pos.x = 
                MOVE_BUTTON_ORIGIN_X
                + 0.5f * (visible_move_button_count - 1) * MOVE_BUTTON_WIDTH
                - 0.5f * (remaining_visible_move_button_count - 1) * (MOVE_BUTTON_STEP_X + MOVE_BUTTON_WIDTH);
            remaining_visible_move_button_count--;
        }
        else {
            moveb_widgets[i]->rect.pos.x = MOVE_BUTTON_ORIGIN_X;
        }
    }
}

void button_notify_deal(void *self, Event event, void *dependencies){
    Button *button = (Button *)self;
    switch(event.type){
        case STATE_EVENT_DEAL: {
            int player_cards_in_hand = event.state.data.hand.player_cards_in_hand;
            int player_hand_value = event.state.data.hand.player_hand_value;
            if (!blackjack(player_cards_in_hand, player_hand_value)){
                button_set_state(button, BUTTON_STATE_DISABLED);
                button->widget.rect.visible = false;
            }
            break;
        }
        case STATE_EVENT_HIT: {
            int player_hand_value = event.state.data.hand.player_hand_value;
            if (bust(player_hand_value)){
                button_set_state(button, BUTTON_STATE_IDLE);
                button->widget.rect.visible = true;
            }
            break;
        }
        case STATE_EVENT_STAND:
            button_set_state(button, BUTTON_STATE_IDLE);
            button->widget.rect.visible = true;
            break;
        case ANIMATION_EVENT_QUEUE_BLOCKING:
            button_set_state(button, BUTTON_STATE_DISABLED);
            break;
        case ANIMATION_EVENT_QUEUE_NONBLOCKING:
            button_restore_prev_state(button);
            break;
        default:
            break;
    }
}

void button_notify_hit(void *self, Event event, void *dependencies){
    Button *button = (Button *)self;
    switch(event.type){
        case STATE_EVENT_DEAL: {
            int player_cards_in_hand = event.state.data.hand.player_cards_in_hand;
            int player_hand_value = event.state.data.hand.player_hand_value;
            if (!blackjack(player_cards_in_hand, player_hand_value)){
                button_set_state(button, BUTTON_STATE_IDLE);
                button->widget.rect.visible = true;
            }
            break;
        }
        case STATE_EVENT_HIT: {
            int player_cards_in_hand = event.state.data.hand.player_cards_in_hand;
            int player_hand_value = event.state.data.hand.player_hand_value;
            if (!can_hit(player_cards_in_hand, player_hand_value)){
                button_set_state(button, BUTTON_STATE_DISABLED);
                if (bust(player_hand_value)){
                    button->widget.rect.visible = false;
                }
            }
            break;
        }
        case STATE_EVENT_STAND:
            button_set_state(button, BUTTON_STATE_DISABLED);
            button->widget.rect.visible = false;
            break;
        case ANIMATION_EVENT_QUEUE_BLOCKING:
            button_set_state(button, BUTTON_STATE_DISABLED);
            break;
        case ANIMATION_EVENT_QUEUE_NONBLOCKING:
            button_restore_prev_state(button);
            break;
        default:
            break;
    }
}

void button_notify_stand(void *self, Event event, void *dependencies){
    Button *button = (Button *)self;
    switch(event.type){
        case STATE_EVENT_DEAL: {
            int player_cards_in_hand = event.state.data.hand.player_cards_in_hand;
            int player_hand_value = event.state.data.hand.player_hand_value;
            if (!blackjack(player_cards_in_hand, player_hand_value)){
                button_set_state(button, BUTTON_STATE_IDLE);
                button->widget.rect.visible = true;
            }
            break;
        }
        case STATE_EVENT_HIT: {
            int player_hand_value = event.state.data.hand.player_hand_value;
            if (bust(player_hand_value)){
                button_set_state(button, BUTTON_STATE_DISABLED);
                button->widget.rect.visible = false;
            }
            break;
        }
        case STATE_EVENT_STAND:
            button_set_state(button, BUTTON_STATE_DISABLED);
            button->widget.rect.visible = false;
            break;
        case ANIMATION_EVENT_QUEUE_BLOCKING:
            button_set_state(button, BUTTON_STATE_DISABLED);
            break;
        case ANIMATION_EVENT_QUEUE_NONBLOCKING:
            button_restore_prev_state(button);
            break;
        default:
            break;
    }
}

void button_notify_bet(void *self, Event event, void *dependencies){
    Button *button = (Button *)self;
    switch (event.type){
        case STATE_EVENT_DEAL: {
            int player_cards_in_hand = event.state.data.hand.player_cards_in_hand;
            int player_hand_value = event.state.data.hand.player_hand_value;
            if (!blackjack(player_cards_in_hand, player_hand_value)){
                button_set_state(button, BUTTON_STATE_DISABLED);
                button->widget.rect.visible = false;
            }
            break;
        } 
        case STATE_EVENT_HIT: {
            int player_hand_value = event.state.data.hand.player_hand_value;
            if (bust(player_hand_value)){
                button_set_state(button, BUTTON_STATE_IDLE);
                button->widget.rect.visible = true;
            }
            break;
        }
        case STATE_EVENT_STAND:
            button_set_state(button, BUTTON_STATE_IDLE);
            button->widget.rect.visible = true;
            break;
        case STATE_EVENT_BET:
            button_set_state(button, BUTTON_STATE_DISABLED);
            button->widget.rect.visible = false;
            break;
        case ANIMATION_EVENT_QUEUE_BLOCKING:
            button_set_state(button, BUTTON_STATE_DISABLED);
            break;
        case ANIMATION_EVENT_QUEUE_NONBLOCKING:
            button_restore_prev_state(button);
            break;
        default:
            break;
    }
}

void button_notify_stack(void *self, Event event, void *dependencies){
    Button *button = (Button *)self;
    switch (event.type){
        case STATE_EVENT_DEAL:
            button_set_state(button, BUTTON_STATE_DISABLED);
            button->widget.rect.visible = false;
            break;
        case STATE_EVENT_BET:
            button_set_state(button, BUTTON_STATE_IDLE);
            button->widget.rect.visible = true;
            break;
        default:
            break;
    }
}

void button_notify_white(void *self, Event event, void *dependencies){
    Button *button = (Button *)self;
    switch (event.type){
        case STATE_EVENT_DEAL:
            button_set_state(button, BUTTON_STATE_DISABLED);
            button->widget.rect.visible = false;
            break;
        case STATE_EVENT_BET:
            button_set_state(button, BUTTON_STATE_IDLE);
            button->widget.rect.visible = true;
            break;
        default:
            break;
    }
}