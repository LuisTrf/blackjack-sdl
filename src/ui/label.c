#include <SDL3_ttf/SDL_ttf.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

#include "../../include/stb_ds.h"
#include "../../include/ui/label.h"
#include "../../include/ui/label_constants.h"
#include "../../include/game/game.h"

void label_update_dimensions(Label *label, font_hash* font_map){
    TTF_GetStringSize(
        hmget(font_map, label->fid), 
        label->txt, 
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

void label_align_x(Label *label, float target_x){
    label->widget.rect.pos.x = target_x - (label->widget.rect.width)/2.f;
}

void label_align_y(Label *label, float target_y){
    label->widget.rect.pos.y = target_y - (label->widget.rect.height)/2.f;
}

void label_notify_dealer_hand(void *self, Event event, void *dependencies){
    Label *label = (Label *)self;
    switch (event.type){
        case STATE_EVENT_DEAL: {
            label_notify_dependencies* label_dependencies = (label_notify_dependencies*)dependencies;
            font_hash* font_map = label_dependencies->font_map;
            int dealer_cards_in_hand = event.state.data.hand.dealer_cards_in_hand;
            int dealer_hand_value = event.state.data.hand.dealer_hand_value;
            int player_cards_in_hand = event.state.data.hand.player_cards_in_hand;
            int player_hand_value = event.state.data.hand.player_hand_value;
            label->widget.rect.pos.x = HAND_LABEL_ORIGIN_X + dealer_cards_in_hand*HAND_LABEL_STEP_X;
            label->widget.rect.visible = true;
            if (blackjack(player_cards_in_hand, player_hand_value)){
                label_write(label, font_map, "%d, DEALER LOSES...", dealer_hand_value);
            }
            else {
                label_write(label, font_map, "?");
            }
            break;
        }
        case STATE_EVENT_HIT: {
            label_notify_dependencies* label_dependencies = (label_notify_dependencies*)dependencies;
            font_hash* font_map = label_dependencies->font_map;
            int dealer_hand_value = event.state.data.hand.dealer_hand_value;
            int player_hand_value = event.state.data.hand.player_hand_value;
            if (bust(player_hand_value)){
                label_write(label, font_map, "%d, DEALER WINS!", dealer_hand_value);
            }
            break;
        }
        case STATE_EVENT_STAND: {
            label_notify_dependencies* label_dependencies = (label_notify_dependencies*)dependencies;
            font_hash* font_map = label_dependencies->font_map;
            int dealer_cards_in_hand = event.state.data.hand.dealer_cards_in_hand;
            int dealer_hand_value = event.state.data.hand.dealer_hand_value;
            int player_hand_value = event.state.data.hand.player_hand_value;
            if (blackjack(dealer_cards_in_hand, dealer_hand_value)){
                label_write(label, font_map, "BLACKJACK!");
                label->widget.rect.visible = true;
            }
            else if (bust(dealer_hand_value)){
                label->widget.rect.pos.x = HAND_LABEL_ORIGIN_X + dealer_cards_in_hand*HAND_LABEL_STEP_X;
                label_write(label, font_map, "%d, BUST!", dealer_hand_value);
            }
            else {
                label->widget.rect.pos.x = HAND_LABEL_ORIGIN_X + dealer_cards_in_hand*HAND_LABEL_STEP_X;
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
            label_notify_dependencies* label_dependencies = (label_notify_dependencies*)dependencies;
            font_hash* font_map = label_dependencies->font_map;
            int player_cards_in_hand = event.state.data.hand.player_cards_in_hand;
            int player_hand_value = event.state.data.hand.player_hand_value;
            label->widget.rect.pos.x = HAND_LABEL_ORIGIN_X + player_cards_in_hand*HAND_LABEL_STEP_X;
            if (blackjack(player_cards_in_hand, player_hand_value)){
                label_write(label, font_map, "BLACKJACK!");
            }
            else {
                label_write(label, font_map, "%d", player_hand_value);
            }
            break;
        }
        case STATE_EVENT_HIT: {
            label_notify_dependencies* label_dependencies = (label_notify_dependencies*)dependencies;
            font_hash* font_map = label_dependencies->font_map;
            int player_cards_in_hand = event.state.data.hand.player_cards_in_hand;
            int player_hand_value = event.state.data.hand.player_hand_value;
            label->widget.rect.pos.x = HAND_LABEL_ORIGIN_X + player_cards_in_hand*HAND_LABEL_STEP_X;
            if (bust(player_hand_value)) {
                label_write(label, font_map, "%d, BUST!", player_hand_value);
            }
            else {
                label_write(label, font_map, "%d", player_hand_value);
            }
            break;
        }
        case STATE_EVENT_STAND: {
            label_notify_dependencies* label_dependencies = (label_notify_dependencies*)dependencies;
            font_hash* font_map = label_dependencies->font_map;
            int dealer_cards_in_hand = event.state.data.hand.dealer_cards_in_hand;
            int dealer_hand_value = event.state.data.hand.dealer_hand_value;
            int player_cards_in_hand = event.state.data.hand.player_cards_in_hand;
            int player_hand_value = event.state.data.hand.player_hand_value;
            label->widget.rect.pos.x = HAND_LABEL_ORIGIN_X + player_cards_in_hand*HAND_LABEL_STEP_X;
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