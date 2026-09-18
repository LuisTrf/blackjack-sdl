#include <SDL3_ttf/SDL_ttf.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

#include "../../vendored/stb/stb_ds.h"
#include "../../include/ui/label.h"
#include "../../include/ui/label_constants.h"
#include "../../include/game/game.h"

void label_update_dimensions(Label *label, font_hash* font_map){
    TTF_GetStringSizeWrapped(
        hmget(font_map, label->fid), 
        label->txt, 
        0,
        0,
        &(label->widget.rect.width), 
        &(label->widget.rect.height)
    );
}

Label* label_create(
    font_hash* font_map,
    float x, float y, 
    bool visible, 
    FONT_ID fid,
    TEXTURE_ID tid
)
{
    Label label = {
        {
            {
                .pos={.x=x, .y=y}, 
                .visible=visible, 
            },
            WIDGET_LABEL, 
        }, 
        fid,
        tid, 
        false,
        ""
    };
    Label* p_label = malloc(sizeof(Label));
    if (p_label==NULL){
        abort();
    }
    *p_label = label;
    label_update_dimensions(p_label, font_map);
    return p_label;
}

void label_destroy(Label *p_label){
    free(p_label);
    p_label=NULL;
}

void label_write(Label *label, font_hash* font_map, const char *fmt, ...){
    va_list args;
    va_start(args, fmt);
    vsnprintf(label->txt, sizeof(label->txt)/sizeof(char), fmt, args);
    va_end(args);
    label_update_dimensions(label, font_map);
    label->_retex = true;
}

void label_notify_dealer_hand(void *self, Event event, void *dependencies){
    Label *label = (Label *)self;
    switch (event.type){
        case STATE_EVENT_DEAL: {
            font_hash* font_map = (font_hash *)dependencies;
            int dealer_cards_in_hand = event.state.data.deal.dealer_cards_in_hand;
            int dealer_hand_value = event.state.data.deal.dealer_hand_value;
            int player_cards_in_hand = event.state.data.deal.player_cards_in_hand;
            int player_hand_value = event.state.data.deal.player_hand_value;
            label->widget.rect.pos.x = HAND_LABEL_ORIGIN_X + (dealer_cards_in_hand - 1)*HAND_LABEL_STEP_X;
            label->widget.rect.visible = true;
            if (blackjack(player_cards_in_hand, player_hand_value)){
                if (blackjack(dealer_cards_in_hand, dealer_hand_value)){
                    label_write(label, font_map, "PUSH.");
                }
                else {
                    label_write(label, font_map, "%d, DEALER LOSES...", dealer_hand_value);
                }
            }
            else {
                label_write(label, font_map, "?");
            }
            rect_align_y((Rect *)label, HAND_LABEL_ORIGIN_Y_DEALER);
            break;
        }
        case STATE_EVENT_HIT: {
            font_hash* font_map = (font_hash *)dependencies;
            int dealer_hand_value = event.state.data.hit.dealer_hand_value;
            int player_hand_value = event.state.data.hit.player_hand_value;
            if (bust(player_hand_value)){
                label_write(label, font_map, "%d, DEALER WINS!", dealer_hand_value);
            }
            break;
        }
        case STATE_EVENT_STAND: {
            font_hash* font_map = (font_hash *)dependencies;
            int dealer_cards_in_hand = event.state.data.stand.dealer_cards_in_hand;
            int dealer_hand_value = event.state.data.stand.dealer_hand_value;
            int player_hand_value = event.state.data.stand.player_hand_value;
            if (blackjack(dealer_cards_in_hand, dealer_hand_value)){
                label_write(label, font_map, "BLACKJACK!");
                label->widget.rect.visible = true;
            }
            else if (bust(dealer_hand_value)){
                label->widget.rect.pos.x = HAND_LABEL_ORIGIN_X + (dealer_cards_in_hand - 1)*HAND_LABEL_STEP_X;
                label_write(label, font_map, "%d, BUST!", dealer_hand_value);
            }
            else {
                label->widget.rect.pos.x = HAND_LABEL_ORIGIN_X + (dealer_cards_in_hand - 1)*HAND_LABEL_STEP_X;
                if (dealer_hand_value > player_hand_value) {
                    label_write(label, font_map, "%d, DEALER WINS!", dealer_hand_value);
                }
                else if (dealer_hand_value < player_hand_value) {
                    label_write(label, font_map, "%d, DEALER LOSES...", dealer_hand_value);
                }
                else {
                    label_write(label, font_map, "%d, PUSH.", dealer_hand_value);
                }
                if (dealer_cards_in_hand == 2) {label->widget.rect.visible = true;}
            }
            break;
        }
        case STATE_EVENT_BET: {
            font_hash* font_map = (font_hash *)dependencies;
            label->widget.rect.pos.x = HAND_LABEL_ORIGIN_X;
            label_write(label, font_map, "");
            label->widget.rect.visible = false;
            break;
        }
        case STATE_EVENT_SPLIT: {
            rect_align_y((Rect *)label, HAND_LABEL_SPLITTING_Y_DEALER);
            break;
        }
        case ANIMATION_EVENT_QUEUE_BLOCKING:
            label->widget.rect.visible = false;
            break;
        case ANIMATION_EVENT_QUEUE_NONBLOCKING:
            label->widget.rect.visible = true;
            break;
        default:
            break;
    }
}

void label_notify_player_hand(void *self, Event event, void *dependencies){
    Label *label = (Label *)self;
    switch(event.type){
        case STATE_EVENT_DEAL: {
            font_hash* font_map = (font_hash *)dependencies;
            int player_cards_in_hand = event.state.data.deal.player_cards_in_hand;
            int player_hand_value = event.state.data.deal.player_hand_value;
            int dealer_cards_in_hand = event.state.data.deal.dealer_cards_in_hand;
            int dealer_hand_value = event.state.data.deal.dealer_hand_value;
            label->widget.rect.pos.x = HAND_LABEL_ORIGIN_X + (player_cards_in_hand - 1)*HAND_LABEL_STEP_X;
            if (blackjack(player_cards_in_hand, player_hand_value)){
                if (blackjack(dealer_cards_in_hand, dealer_hand_value)){
                    label_write(label, font_map, "PUSH.");
                }
                else {
                    label_write(label, font_map, "BLACKJACK!");
                }
            }
            else {
                label_write(label, font_map, "%d", player_hand_value);
            }
            rect_align_y((Rect *)label, HAND_LABEL_ORIGIN_Y_PLAYER);
            break;
        }
        case STATE_EVENT_HIT: {
            font_hash* font_map = (font_hash *)dependencies;
            int player_cards_in_hand = event.state.data.hit.player_cards_in_hand;
            int player_hand_value = event.state.data.hit.player_hand_value;
            label->widget.rect.pos.x = HAND_LABEL_ORIGIN_X + (player_cards_in_hand - 1)*HAND_LABEL_STEP_X;
            if (bust(player_hand_value)) {
                label_write(label, font_map, "%d, BUST!", player_hand_value);
            }
            else {
                label_write(label, font_map, "%d", player_hand_value);
            }
            break;
        }
        case STATE_EVENT_STAND: {
            font_hash* font_map = (font_hash *)dependencies;
            int dealer_cards_in_hand = event.state.data.stand.dealer_cards_in_hand;
            int dealer_hand_value = event.state.data.stand.dealer_hand_value;
            int player_cards_in_hand = event.state.data.stand.player_cards_in_hand;
            int player_hand_value = event.state.data.stand.player_hand_value;
            label->widget.rect.pos.x = HAND_LABEL_ORIGIN_X + (player_cards_in_hand - 1)*HAND_LABEL_STEP_X;
            if (blackjack(dealer_cards_in_hand, dealer_hand_value)){
                label_write(label, font_map, "%d, PLAYER LOSES...", player_hand_value);
            }
            else if (player_hand_value > dealer_hand_value) {
                label_write(label, font_map, "%d, PLAYER WINS!", player_hand_value);
            }
            else if (player_hand_value < dealer_hand_value) {
                if (bust(dealer_hand_value)){
                    label_write(label, font_map, "%d, PLAYER WINS!", player_hand_value);
                }
                else {
                    label_write(label, font_map, "%d, PLAYER LOSES...", player_hand_value);
                }
            }
            else {
                label_write(label, font_map, "%d, PUSH.", player_hand_value);
            }
            break;
        }
        case STATE_EVENT_BET: {
            font_hash* font_map = (font_hash *)dependencies;
            label->widget.rect.pos.x = HAND_LABEL_ORIGIN_X;
            label_write(label, font_map, "");
            label->widget.rect.visible = false;
            break;
        }
        case STATE_EVENT_SPLIT: {
            font_hash* font_map = (font_hash *)dependencies;
            int player_hand_value = event.state.data.split.player_hand_value;
            label->widget.rect.pos.x = HAND_LABEL_ORIGIN_X;
            label_write(label, font_map, "%d", player_hand_value);
            rect_align_y((Rect *)label, HAND_LABEL_SPLITTING_Y_PLAYER);
            break;
        }
        case ANIMATION_EVENT_QUEUE_BLOCKING:
            label->widget.rect.visible = false;
            break;
        case ANIMATION_EVENT_QUEUE_NONBLOCKING:
            label->widget.rect.visible = true;
            break;
        default:
            break;
    }
}

void label_notify_player_money(void *self, Event event, void *dependencies){
    Label *label = (Label *)self;
    switch (event.type){
        case STATE_EVENT_BET_PAYOUT: {
            font_hash* font_map = (font_hash *)dependencies;
            float money = event.state.data.bet_payout.money;
            label_write(label, font_map, "MONEY: $%.2f", money);
            break;
        }
        case STATE_EVENT_CHEQUE_PUSH_SENT: {
            font_hash* font_map = (font_hash *)dependencies;
            float money = event.state.data.cheque_push_sent.money;
            label_write(label, font_map, "MONEY: $%.2f", money);
            break;
        }
        case STATE_EVENT_CHEQUE_POP_RECEIVED: {
            font_hash* font_map = (font_hash *)dependencies;
            float money = event.state.data.cheque_pop_received.money;
            label_write(label, font_map, "MONEY: $%.2f", money);
            break;
        }
        case STATE_EVENT_SPLIT: {
            font_hash* font_map = (font_hash *)dependencies;
            float money = event.state.data.split.money;
            label_write(label, font_map, "MONEY: $%.2f", money);
            break;
        }
        case STATE_EVENT_DOUBLE_DOWN: {
            font_hash* font_map = (font_hash *)dependencies;
            float money = event.state.data.double_down.money;
            label_write(label, font_map, "MONEY: $%.2f", money);
            break;
        }
        case STATE_EVENT_INSURANCE: {
            font_hash* font_map = (font_hash *)dependencies;
            float money = event.state.data.insurance.money;
            label_write(label, font_map, "MONEY: $%.2f", money);
            break;
        }
        default:
            break;
    }
}

void label_notify_player_bet(void *self, Event event, void *dependencies){
    Label *label = (Label *)self;
    switch (event.type){
        case STATE_EVENT_BET: {
            font_hash* font_map = (font_hash *)dependencies;
            label_write(label, font_map, "BET: $0.00");
            rect_align_x((Rect *)label, BET_LABEL_ORIGIN_X);
            label->widget.rect.pos.y = BET_LABEL_ORIGIN_Y;
            label->widget.rect.visible = true;
            break;
        }
        case STATE_EVENT_GAME_STATE:
            if (event.state.data.game_state.game_state != GAME_STATE_BETTING){
                label->widget.rect.pos = (vec2){0,50};
            }
            break;
        case STATE_EVENT_STAND: 
            if (label->widget.rect.visible) {
                label->widget.rect.visible = false;
            }
            break;
        case STATE_EVENT_HIT: {
            int player_hand_value = event.state.data.hit.player_hand_value;
            if (label->widget.rect.visible && bust(player_hand_value)) {
                label->widget.rect.visible = false;
            }
            break;
        }
        case STATE_EVENT_SPLIT_HIT: {
            font_hash* font_map = (font_hash *)dependencies;
            int player_cards_in_split_hand = event.state.data.split_hit.player_cards_in_split_hand;
            int player_split_hand_value = event.state.data.split_hit.player_split_hand_value;
            float bet = event.state.data.split_hit.bet;
            if (bust(player_split_hand_value)){
                label_write(label, font_map, "BET: $%.2f", bet);
            }
            break;
        }
        case STATE_EVENT_CHEQUE_PUSH_RECEIVED: {
            font_hash* font_map = (font_hash *)dependencies;
            float bet = event.state.data.cheque_push_received.bet;
            label_write(label, font_map, "BET: $%.2f", bet);
            rect_align_x((Rect *)label, BET_LABEL_ORIGIN_X);
            break;
        }
        case STATE_EVENT_CHEQUE_POP_SENT: {
            font_hash* font_map = (font_hash *)dependencies;
            float bet = event.state.data.cheque_pop_sent.bet;
            label_write(label, font_map, "BET: $%.2f", bet);
            rect_align_x((Rect *)label, BET_LABEL_ORIGIN_X);
            break;
        }
        case STATE_EVENT_SPLIT: {
            font_hash* font_map = (font_hash *)dependencies;
            float bet = event.state.data.split.bet;
            float split_bet = event.state.data.split.split_bet;
            label_write(label, font_map, "BET: $%.2f\nSPLIT BET: $%.2f", bet, split_bet);
            break;
        }
        case STATE_EVENT_BET_PAYOUT: {
            font_hash* font_map = (font_hash *)dependencies;
            label_write(label, font_map, "");
            break;
        }
        case STATE_EVENT_DOUBLE_DOWN: {
            font_hash* font_map = (font_hash *)dependencies;
            GAME_STATE game_state = event.state.data.double_down.game_state;
            GAME_STATE prev_game_state = event.state.data.double_down.prev_game_state;
            float bet = event.state.data.double_down.bet;
            float split_bet = event.state.data.double_down.split_bet;
            if (game_state == GAME_STATE_BETTING_PLAYING){
                if (prev_game_state == GAME_STATE_PLAYING_SPLIT){
                    label_write(label, font_map, "BET: 2x$%.2f\nSPLIT BET: 2x$%.2f", bet/2.f, split_bet/2.f);
                }
                else {
                    label_write(label, font_map, "BET: 2x$%.2f", bet/2.f);
                }
            }
            else if (game_state == GAME_STATE_PLAYING_SPLIT){
                label_write(label, font_map, "BET: $%.2f\nSPLIT BET: 2x$%.2f", bet, split_bet/2.f);
            }
            break;
        }
        case STATE_EVENT_INSURANCE: 
            if (label->widget.rect.visible) {
                    label->widget.rect.visible = false;
                }
            break;
        default:
            break;
    }
}

void label_notify_player_split_hand(void *self, Event event, void *dependencies){
    Label *label = (Label *)self;
    switch (event.type){
        case STATE_EVENT_SPLIT: {
            font_hash* font_map = (font_hash *)dependencies;
            int player_split_hand_value = event.state.data.split.player_split_hand_value;
            label_write(label, font_map, "%d", player_split_hand_value);
            rect_align_y((Rect *)label, SPLIT_HAND_LABEL_SPLITTING_Y_PLAYER);
            rect_vis_set((Rect *)label, true);
            break;
        }
        case STATE_EVENT_SPLIT_HIT: {
            font_hash* font_map = (font_hash *)dependencies;
            int player_cards_in_split_hand = event.state.data.split_hit.player_cards_in_split_hand;
            int player_split_hand_value = event.state.data.split_hit.player_split_hand_value;
            label->widget.rect.pos.x = HAND_LABEL_ORIGIN_X + (player_cards_in_split_hand - 1)*HAND_LABEL_STEP_X;
            if (bust(player_split_hand_value)) {
                label_write(label, font_map, "%d, BUST!", player_split_hand_value);
            }
            else {
                label_write(label, font_map, "%d", player_split_hand_value);
            }
            break;
        }
        case STATE_EVENT_DEAL:
        case STATE_EVENT_BET: {
            font_hash* font_map = (font_hash *)dependencies;
            label->widget.rect.pos.x = HAND_LABEL_ORIGIN_X;
            label_write(label, font_map, "");
            rect_vis_set((Rect *)label, false);
            break;
        }
        case STATE_EVENT_HIT: {
            if (!label->widget.rect.visible) {return;}
            font_hash* font_map = (font_hash *)dependencies;
            int dealer_cards_in_hand = event.state.data.stand.dealer_cards_in_hand;
            int dealer_hand_value = event.state.data.stand.dealer_hand_value;
            int player_hand_value = event.state.data.stand.player_hand_value;
            int player_split_hand_value = event.state.data.stand.player_split_hand_value;
            if (bust(player_hand_value)){
                if (blackjack(dealer_cards_in_hand, dealer_hand_value)){
                    label_write(label, font_map, "%d, PLAYER LOSES...", player_split_hand_value);
                }
                else if (player_split_hand_value > dealer_hand_value) {
                    label_write(label, font_map, "%d, PLAYER WINS!", player_split_hand_value);
                }
                else if (player_split_hand_value < dealer_hand_value) {
                    if (bust(dealer_hand_value)){
                        label_write(label, font_map, "%d, PLAYER WINS!", player_split_hand_value);
                    }
                    else {
                        label_write(label, font_map, "%d, PLAYER LOSES...", player_split_hand_value);
                    }
                }
                else {
                    label_write(label, font_map, "%d, PUSH.", player_split_hand_value);
                }
            }
            break;
        }
        case STATE_EVENT_STAND: {
            if (!label->widget.rect.visible) {return;}
            font_hash* font_map = (font_hash *)dependencies;
            int dealer_cards_in_hand = event.state.data.stand.dealer_cards_in_hand;
            int dealer_hand_value = event.state.data.stand.dealer_hand_value;
            int player_cards_in_hand = event.state.data.stand.player_cards_in_hand;
            int player_hand_value = event.state.data.stand.player_hand_value;
            int player_split_hand_value = event.state.data.stand.player_split_hand_value;
            if (blackjack(dealer_cards_in_hand, dealer_hand_value)){
                label_write(label, font_map, "%d, PLAYER LOSES...", player_split_hand_value);
            }
            else if (player_split_hand_value > dealer_hand_value) {
                label_write(label, font_map, "%d, PLAYER WINS!", player_split_hand_value);
            }
            else if (player_split_hand_value < dealer_hand_value) {
                if (bust(dealer_hand_value)){
                    label_write(label, font_map, "%d, PLAYER WINS!", player_split_hand_value);
                }
                else {
                    label_write(label, font_map, "%d, PLAYER LOSES...", player_split_hand_value);
                }
            }
            else {
                label_write(label, font_map, "%d, PUSH.", player_split_hand_value);
            }
            break;
        }
        case ANIMATION_EVENT_QUEUE_BLOCKING:
            rect_hide((Rect *)label);
            break;
        case ANIMATION_EVENT_QUEUE_NONBLOCKING:
            rect_unhide((Rect *)label);
            break;
        default:
            break;
    }
}