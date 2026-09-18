#include <stdbool.h>
#include <stdlib.h>

#include "../../include/ui/button_constants.h"
#include "../../include/event/event_types.h"
#include "../../include/ui/button.h"
#include "../../vendored/stb/stb_ds.h"
#include "../../include/game/game.h"

Button* button_create(
    float x, float y, 
    int width, int height, 
    int sprite_width, int sprite_height,
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
                .sprite_width=sprite_width,
                .sprite_height=sprite_height,
                .visible=visible, 
            },
            .wtype=WIDGET_BUTTON,
        }, 
        .release_event=(Event){.input={.type=release_eventtype}}, 
        ._state=button_state_initial, 
        ._prev_state=_BUTTON_STATE_NONE, 
        .tid=tid
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
        case STATE_EVENT_CHEQUE_POP_SENT: {
            float bet = event.state.data.cheque_pop_sent.bet;
            if (bet == 0){
                button_set_state(button, BUTTON_STATE_DISABLED);
            }
            else if (button_get_state(button) == BUTTON_STATE_DISABLED && bet > 0){
                button_set_state(button, BUTTON_STATE_IDLE);
            }
            break;
        }
        case STATE_EVENT_CHEQUE_PUSH_RECEIVED: {
            float bet = event.state.data.cheque_pop_sent.bet;
            if (bet == 0){
                button_set_state(button, BUTTON_STATE_DISABLED);
            }
            else if (button_get_state(button) == BUTTON_STATE_DISABLED && bet > 0){
                button_set_state(button, BUTTON_STATE_IDLE);
            }
            break;
        }
        case STATE_EVENT_BET: {
            button_set_state(button, BUTTON_STATE_DISABLED);
            break;
        }
        case STATE_EVENT_DEAL: {
            int player_cards_in_hand = event.state.data.deal.player_cards_in_hand;
            int player_hand_value = event.state.data.deal.player_hand_value;
            GAME_STATE game_state = event.state.data.deal.game_state;
            char dealer_first_card_rank = event.state.data.deal.dealer_first_card_rank;
            if (
                !blackjack(player_cards_in_hand, player_hand_value)
                || (
                    blackjack(player_cards_in_hand, player_hand_value)
                    && game_state == GAME_STATE_BETTING_PLAYING
                    && dealer_first_card_rank == 'A'
                )
            ){
                button_set_state(button, BUTTON_STATE_DISABLED);
                button->widget.rect.visible = false;
            }
            break;
        }
        case STATE_EVENT_HIT: {
            int player_hand_value = event.state.data.hit.player_hand_value;
            if (bust(player_hand_value)){
                button_set_state(button, BUTTON_STATE_IDLE);
                button->widget.rect.visible = true;
            }
            break;
        }
        case STATE_EVENT_INSURANCE:
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
            int player_cards_in_hand = event.state.data.deal.player_cards_in_hand;
            int player_hand_value = event.state.data.deal.player_hand_value;
            GAME_STATE game_state = event.state.data.deal.game_state;
            char dealer_first_card_rank = event.state.data.deal.dealer_first_card_rank;
            if (!blackjack(player_cards_in_hand, player_hand_value)){
                button_set_state(button, BUTTON_STATE_IDLE);
                button->widget.rect.visible = true;
            }
            else if (
                blackjack(player_cards_in_hand, player_hand_value)
                && game_state == GAME_STATE_BETTING_PLAYING
                && dealer_first_card_rank == 'A'
            ){
                button_set_state(button, BUTTON_STATE_DISABLED);
                button->widget.rect.visible = true;
            }
            break;
        }
        case STATE_EVENT_HIT: {
            int player_cards_in_hand = event.state.data.hit.player_cards_in_hand;
            int player_hand_value = event.state.data.hit.player_hand_value;
            if (!can_hit(player_cards_in_hand, player_hand_value)){
                button_set_state(button, BUTTON_STATE_DISABLED);
                if (bust(player_hand_value)){
                    button->widget.rect.visible = false;
                }
            }
            break;
        }
        case STATE_EVENT_SPLIT_HIT: {
            int player_split_hand_value = event.state.data.split_hit.player_split_hand_value;
            if (player_split_hand_value == 21){
                button_set_state(button, BUTTON_STATE_DISABLED);
            }
            break;
        }
        case STATE_EVENT_INSURANCE:
        case STATE_EVENT_STAND:
            button_set_state(button, BUTTON_STATE_DISABLED);
            button->widget.rect.visible = false;
            break;
        case STATE_EVENT_SPLIT_STAND:
            button_set_state(button, BUTTON_STATE_IDLE);
            button->widget.rect.visible = true;
            break;
        case STATE_EVENT_DOUBLE_DOWN: 
            button_set_state(button, BUTTON_STATE_DISABLED);
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
            int player_cards_in_hand = event.state.data.deal.player_cards_in_hand;
            int player_hand_value = event.state.data.deal.player_hand_value;
            GAME_STATE game_state = event.state.data.deal.game_state;
            char dealer_first_card_rank = event.state.data.deal.dealer_first_card_rank;
            if (
                !blackjack(player_cards_in_hand, player_hand_value)
                || (
                    blackjack(player_cards_in_hand, player_hand_value)
                    && game_state == GAME_STATE_BETTING_PLAYING
                    && dealer_first_card_rank == 'A'
                )
            ){
                button_set_state(button, BUTTON_STATE_IDLE);
                button->widget.rect.visible = true;
            }
            break;
        }
        case STATE_EVENT_HIT: {
            int player_hand_value = event.state.data.hit.player_hand_value;
            if (bust(player_hand_value)){
                button_set_state(button, BUTTON_STATE_DISABLED);
                button->widget.rect.visible = false;
            }
            break;
        }
        case STATE_EVENT_INSURANCE:
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
            int player_cards_in_hand = event.state.data.deal.player_cards_in_hand;
            int player_hand_value = event.state.data.deal.player_hand_value;
            GAME_STATE game_state = event.state.data.deal.game_state;
            char dealer_first_card_rank = event.state.data.deal.dealer_first_card_rank;
            if (
                !blackjack(player_cards_in_hand, player_hand_value)
                || (
                    blackjack(player_cards_in_hand, player_hand_value)
                    && game_state == GAME_STATE_BETTING_PLAYING
                    && dealer_first_card_rank == 'A'
                )
            ){
                button_set_state(button, BUTTON_STATE_DISABLED);
                button->widget.rect.visible = false;
            }
            else {
                button_set_state(button, BUTTON_STATE_IDLE);
                button->widget.rect.visible = true;
            }
            break;
        } 
        case STATE_EVENT_HIT: {
            int player_hand_value = event.state.data.hit.player_hand_value;
            float money = event.state.data.hit.money;
            if (bust(player_hand_value) && money > 0){
                button_set_state(button, BUTTON_STATE_IDLE);
                button->widget.rect.visible = true;
            }
            break;
        }
        case STATE_EVENT_INSURANCE:
            button_set_state(button, BUTTON_STATE_IDLE);
            button->widget.rect.visible = true;
            break;
        case STATE_EVENT_STAND: {
            float money = event.state.data.hit.money;
            if (money > 0){
                button_set_state(button, BUTTON_STATE_IDLE);
                button->widget.rect.visible = true;
            }
            break;
        }
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

void button_notify_split(void *self, Event event, void *dependencies){
    Button *button = (Button *)self;
    switch (event.type){
        case STATE_EVENT_SPLIT_POSSIBLE: 
            button_set_state(button, BUTTON_STATE_IDLE);
            button->widget.rect.visible = true;
            break;
        case STATE_EVENT_HIT:
        case STATE_EVENT_STAND:
        case STATE_EVENT_SPLIT:
        case STATE_EVENT_INSURANCE:
        case STATE_EVENT_DOUBLE_DOWN:
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

void button_notify_double_down(void *self, Event event, void *dependencies){
    Button *button = (Button *)self;
    switch (event.type){
        case STATE_EVENT_DOUBLE_DOWN_POSSIBLE:
            button_set_state(button, BUTTON_STATE_IDLE);
            button->widget.rect.visible = true;
            break;
        case STATE_EVENT_SPLIT: {
            float money = event.state.data.split.money;
            float bet = event.state.data.split.bet;
            /*
            bet placed on hand 1 and hand 2 are identical;
            can dd on hand 1 -> can dd on hand 2
            */
            if (bet > money){
                button_set_state(button, BUTTON_STATE_DISABLED);
                button->widget.rect.visible = false;
            }
            break;
        }
        case STATE_EVENT_DEAL: {
            int player_cards_in_hand = event.state.data.deal.player_cards_in_hand;
            int player_hand_value = event.state.data.deal.player_hand_value;
            GAME_STATE game_state = event.state.data.deal.game_state;
            char dealer_first_card_rank = event.state.data.deal.dealer_first_card_rank;
            if (
                blackjack(player_cards_in_hand, player_hand_value)
                && (
                    !blackjack(player_cards_in_hand, player_hand_value)
                    || game_state != GAME_STATE_BETTING_PLAYING
                    || dealer_first_card_rank != 'A'
                )
            ){
                button_set_state(button, BUTTON_STATE_DISABLED);
                button->widget.rect.visible = false;
            }
            break;
        }
        case STATE_EVENT_SPLIT_STAND:
            button_set_state(button, BUTTON_STATE_IDLE);
            button->widget.rect.visible = true;
            break;
        case STATE_EVENT_DOUBLE_DOWN:
        case STATE_EVENT_INSURANCE:
        case STATE_EVENT_HIT:
        case STATE_EVENT_SPLIT_HIT:
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

void button_notify_insurance(void *self, Event event, void *dependencies){
    Button *button = (Button *)self;
    switch (event.type){
        case STATE_EVENT_INSURANCE_POSSIBLE: 
            button_set_state(button, BUTTON_STATE_IDLE);
            button->widget.rect.visible = true;
            break;
        case STATE_EVENT_INSURANCE:
        case STATE_EVENT_HIT:
        case STATE_EVENT_STAND:
        case STATE_EVENT_SPLIT:
        case STATE_EVENT_DOUBLE_DOWN:
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
            button->tid = TEXTURE_ID_NULL;
            button_set_state(button, BUTTON_STATE_IDLE);
            button->widget.rect.visible = true;
            break;
        case STATE_EVENT_CHEQUE_PUSH_RECEIVED:
            button->tid = event.state.data.cheque_push_received.tid;
            if (button_get_state(button) == BUTTON_STATE_DISABLED){
                button_set_state(button, BUTTON_STATE_IDLE);
                button->widget.rect.visible = true;
            }
            break;
        case STATE_EVENT_CHEQUE_POP_SENT: 
            button->tid = event.state.data.cheque_pop_sent.tid;
            if (event.state.data.cheque_pop_sent.bet == 0){
                button_set_state(button, BUTTON_STATE_DISABLED);
                button->widget.rect.visible = false;
            }
        default:
            break;
    }
}

void button_notify_cheque(void *self, Event event, void *dependencies, CHEQUE_VALUE val){
    Button *button = (Button *)self;
    switch (event.type){
        case STATE_EVENT_DEAL:
            button_set_state(button, BUTTON_STATE_DISABLED);
            button->widget.rect.visible = false;
            break;
        case STATE_EVENT_BET:
            if (event.state.data.bet.money < val){
                button_set_state(button, BUTTON_STATE_DISABLED);
                button->widget.rect.visible = false;
            }
            else {
                button_set_state(button, BUTTON_STATE_IDLE);
                button->widget.rect.visible = true;
            }
            break;
        case STATE_EVENT_CHEQUE_PUSH_SENT:
        case STATE_EVENT_CHEQUE_POP_RECEIVED:
            if (event.state.data.cheque_push_sent.money < val){
                button_set_state(button, BUTTON_STATE_DISABLED);
                button->widget.rect.visible = false;
            }
            else {
                button_set_state(button, BUTTON_STATE_IDLE);
                button->widget.rect.visible = true;
            }
            break;
        default:
            break;
    }
}

void button_notify_white(void *self, Event event, void *dependencies){
    button_notify_cheque(self, event, dependencies, CHEQUE_VALUE_ONE);
}

void button_notify_red(void *self, Event event, void *dependencies){
    button_notify_cheque(self, event, dependencies, CHEQUE_VALUE_FIVE);
}

void button_notify_blue(void *self, Event event, void *dependencies){
    button_notify_cheque(self, event, dependencies, CHEQUE_VALUE_TEN);
}

void button_notify_green(void *self, Event event, void *dependencies){
    button_notify_cheque(self, event, dependencies, CHEQUE_VALUE_TWENTY_FIVE);
}

void button_notify_black(void *self, Event event, void *dependencies){
    button_notify_cheque(self, event, dependencies, CHEQUE_VALUE_HUNDRED);
}

void button_notify_purple(void *self, Event event, void *dependencies){
    button_notify_cheque(self, event, dependencies, CHEQUE_VALUE_FIVE_HUNDRED);
}

void button_notify_yellow(void *self, Event event, void *dependencies){
    button_notify_cheque(self, event, dependencies, CHEQUE_VALUE_ONE_K);
}

void button_notify_orange(void *self, Event event, void *dependencies){
    button_notify_cheque(self, event, dependencies, CHEQUE_VALUE_FIVE_K);
}

void button_notify_redblue(void *self, Event event, void *dependencies){
    button_notify_cheque(self, event, dependencies, CHEQUE_VALUE_TWENTY_FIVE_K);
}

void button_notify_gold(void *self, Event event, void *dependencies){
    button_notify_cheque(self, event, dependencies, CHEQUE_VALUE_HUNDRED_K);
}