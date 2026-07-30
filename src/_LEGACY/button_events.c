#include <stdio.h>
#include <stdbool.h>
#include "../include/card_constants.h"
#include "../include/buttons.h"
#include "../include/labels.h"
#include "../include/game_objects.h"
#include "../include/game.h"
#include "../include/animate.h"
#include "../include/events.h"

static void set_new_game_action_button_states(void){
    hide_and_disable_all_action_buttons();
    set_state(&deal_button, IDLE);
    deal_button.obj.visible=true;
    if (can_player_bet()){
        set_state(&bet_button, IDLE);
        bet_button.obj.visible=true;
    }
}

static void new_game(void){
    set_game_state(NEW);
    Event event = {.type=NEW_GAME_EVENT, NULL};
    enqueue_event(event);
    set_new_game_action_button_states();
}

static void dealer_draw_two(void){
    Card* card = dealer_hit();
    enqueue_anim(create_anim(
        &(card->obj), 
        DECK_X_ORIGIN-(get_player_cards_in_hand()+get_dealer_cards_in_hand()),
        DECK_Y_ORIGIN+(get_player_cards_in_hand()+get_dealer_cards_in_hand()),
        CARD_X_ORIGIN,
        CARD_Y_ORIGIN_DEALER,
        card_draw_anim
    ));
    card = dealer_hit();
    enqueue_anim(create_anim(
        &(card->obj), 
        DECK_X_ORIGIN-(get_player_cards_in_hand()+get_dealer_cards_in_hand()),
        DECK_Y_ORIGIN+(get_player_cards_in_hand()+get_dealer_cards_in_hand()),
        CARD_X_ORIGIN+CARD_STEP_X,
        CARD_Y_ORIGIN_DEALER,
        card_draw_anim)
    );
}

static void update_dealer_hand_label_geometry(void){
    update_label_dimensions(&dealer_hand_label);
    dealer_hand_label.obj.x = CARD_X_ORIGIN+CARD_STEP_X * (get_dealer_cards_in_hand()-1) + CARD_WIDTH + HAND_FONT_LABEL_X_OFFSET_FROM_HAND;
    align_label_y(&dealer_hand_label, CARD_Y_ORIGIN_DEALER + CARD_HEIGHT/2.f);
}

static void player_draw_two(void){
    Card *card = player_hit();
    enqueue_anim(create_anim(
        &(card->obj), 
        DECK_X_ORIGIN-(get_player_cards_in_hand()+get_dealer_cards_in_hand()),
        DECK_Y_ORIGIN+(get_player_cards_in_hand()+get_dealer_cards_in_hand()),
        CARD_X_ORIGIN,
        CARD_Y_ORIGIN_PLAYER,
        card_draw_anim)
    );
    card = player_hit();
    enqueue_anim(create_anim(
        &(card->obj), 
        DECK_X_ORIGIN-(get_player_cards_in_hand()+get_dealer_cards_in_hand()),
        DECK_Y_ORIGIN+(get_player_cards_in_hand()+get_dealer_cards_in_hand()),
        CARD_X_ORIGIN+CARD_STEP_X,
        CARD_Y_ORIGIN_PLAYER,
        card_draw_anim)
    );
}

static void update_player_hand_label_geometry(void){
    player_hand_label.obj.x = CARD_X_ORIGIN+CARD_STEP_X * (get_player_cards_in_hand()-1) + CARD_WIDTH + HAND_FONT_LABEL_X_OFFSET_FROM_HAND;
    update_label_dimensions(&player_hand_label);
    align_label_y(&player_hand_label, CARD_Y_ORIGIN_PLAYER + CARD_HEIGHT/2.f);
}

static void set_other_action_button_states_on_deal_released(void){
    set_state(&hit_button, IDLE);
    hit_button.obj.visible = true;
    set_state(&stand_button, IDLE);
    stand_button.obj.visible = true;
    set_state(&bet_button, DISABLED);
    bet_button.obj.visible = false;
    if (get_prev_game_state()==BETTING){
        set_state(&stack_button, DISABLED);
        stack_button.obj.visible=false;
        if (can_double_down()){
            set_state(&double_down_button, IDLE);
            double_down_button.obj.visible = true;
        }
    }
}

static void animate_stack_texture_and_bet_label(void){
    TexturedGameObject *tex_obj = create_textured_game_object(
        stack_button.obj.x,
        stack_button.obj.y,
        stack_button.obj.width,
        stack_button.obj.height,
        true,
        stack_button.spritesheet
    );
    enqueue_anim(create_anim(
        &(tex_obj->obj),
        tex_obj->obj.x,
        tex_obj->obj.y,
        WINDOW_WIDTH - (stack_button.obj.width * 1.5f),
        WINDOW_HEIGHT/2.f - (stack_button.obj.height)/2.f,
        move_030_anim
    ));
    add_anim(create_anim(
        &(player_bet_label.obj),
        player_bet_label.obj.x,
        player_bet_label.obj.y,
        WINDOW_WIDTH - stack_button.obj.width - (player_bet_label.obj.width / 2.f),
        WINDOW_HEIGHT/2.f - (stack_button.obj.height)/2.f - player_bet_label.obj.height - 20,
        move_030_anim
    ));
}

static void disable_player_bet_label(void){
    player_bet_label.txt[0]=0;
    update_label_dimensions(&player_bet_label);
    player_bet_label.obj.visible=false;
}

void on_deal_released(Button *self){
    set_state(&deal_button, DISABLED);
    deal_button.obj.visible = false;
    set_game_state(PLAYING);
    set_other_action_button_states_on_deal_released();
    if (get_prev_game_state()!=BETTING){
        reset_game_state();
    }
    else{
        hide_and_disable_all_valued_buttons();
        if (peek_bet()!=_NONE_CHIP_VALUATION){
            animate_stack_texture_and_bet_label();
        }
        else{
            disable_player_bet_label();
        }
    }
    dealer_draw_two();
    if (get_prev_game_state()==BETTING && can_insure()){
        set_state(&insurance_button, IDLE);
        insurance_button.obj.visible=true;
    }
    snprintf(dealer_hand_label.txt, sizeof(dealer_hand_label.txt)/sizeof(char), "%d", get_dealer_hand_value());
    update_dealer_hand_label_geometry();
    player_draw_two();
    if (player_blackjack()){
        snprintf(player_hand_label.txt, sizeof(player_hand_label.txt)/sizeof(char), "PLAYER BLACKJACK!");
        if (get_prev_game_state()==BETTING){
            add_to_player_money(PLAYER_BLACKJACK_BET_PAYOUT*get_player_bet());
            snprintf(player_money_label.txt, sizeof(player_money_label.txt)/sizeof(char), "$%.2lf", get_player_money());
            update_label_dimensions(&player_money_label);
        }
        new_game();
    }
    else {
        snprintf(player_hand_label.txt, sizeof(player_hand_label.txt)/sizeof(char), "%d", get_player_hand_value());;
    }
    update_player_hand_label_geometry();
}

static void player_draw_one(void){
    Card* card = player_hit();
    enqueue_anim(create_anim(
        &(card->obj), 
        DECK_X_ORIGIN-(get_player_cards_in_hand()+get_dealer_cards_in_hand()),
        DECK_Y_ORIGIN+(get_player_cards_in_hand()+get_dealer_cards_in_hand()),
        CARD_X_ORIGIN+CARD_STEP_X*(get_player_cards_in_hand()-1),
        CARD_Y_ORIGIN_PLAYER,
        card_draw_anim)
    );
}

void on_hit_released(Button *self){
    set_state(&hit_button, IDLE);
    if (get_prev_game_state()==BETTING){
        set_state(&double_down_button, DISABLED);
        double_down_button.obj.visible = false;
        set_state(&insurance_button, DISABLED);
        insurance_button.obj.visible=false;
    }
    if (player_can_hit()){
        player_draw_one();
        snprintf(player_hand_label.txt, sizeof(player_hand_label.txt)/sizeof(char), "%d", get_player_hand_value());
        if (player_bust()){
            reveal_second_dealer_card();
            snprintf(player_hand_label.txt, sizeof(player_hand_label.txt)/sizeof(char), "%d, BUST!", get_player_hand_value());
            snprintf(dealer_hand_label.txt, sizeof(dealer_hand_label.txt)/sizeof(char), "DEALER WINS!");
            update_label_dimensions(&dealer_hand_label);
            new_game();
        }
        else if (get_player_cards_in_hand()==5){
            reveal_second_dealer_card();
            snprintf(player_hand_label.txt, sizeof(player_hand_label.txt)/sizeof(char), "5 CARDS, PLAYER WINS!");
            if (get_prev_game_state()==BETTING){
                add_to_player_money(STANDARD_BET_PAYOUT*get_player_bet());
                snprintf(player_money_label.txt, sizeof(player_money_label.txt)/sizeof(char), "$%.2lf", get_player_money());
                update_label_dimensions(&player_money_label);
            }
            new_game();
        }
        else if (get_player_hand_value()==21) {
            set_state(&hit_button, DISABLED);
            hit_button.obj.visible=false;
        }
        update_player_hand_label_geometry();
    }
}

static void dealer_draw_one(void){
    Card* card = dealer_hit();
    enqueue_anim(create_anim(
        &(card->obj), 
        DECK_X_ORIGIN-(get_player_cards_in_hand()+get_dealer_cards_in_hand()),
        DECK_Y_ORIGIN+(get_player_cards_in_hand()+get_dealer_cards_in_hand()),
        CARD_X_ORIGIN+CARD_STEP_X*(get_dealer_cards_in_hand()-1),
        CARD_Y_ORIGIN_DEALER,
        card_draw_anim)
    );
}

static void write_labels_if_none_bust(void){
    if (get_dealer_hand_value() < get_player_hand_value()){
        snprintf(dealer_hand_label.txt, sizeof(dealer_hand_label.txt)/sizeof(char), "%d, PLAYER WINS!", get_dealer_hand_value());
        snprintf(player_hand_label.txt, sizeof(player_hand_label.txt)/sizeof(char), "%d, PLAYER WINS!", get_player_hand_value());
        if (get_prev_game_state()==BETTING){
            snprintf(player_money_label.txt, sizeof(player_money_label.txt)/sizeof(char), "$%.2lf", get_player_money());
        }
    }
    else if (get_dealer_hand_value() > get_player_hand_value()){
        snprintf(dealer_hand_label.txt, sizeof(dealer_hand_label.txt)/sizeof(char), "%d, DEALER WINS!", get_dealer_hand_value());
        snprintf(player_hand_label.txt, sizeof(player_hand_label.txt)/sizeof(char), "%d, DEALER WINS!", get_player_hand_value());
    }
    else {
        snprintf(dealer_hand_label.txt, sizeof(dealer_hand_label.txt)/sizeof(char), "%d, PUSH.", get_dealer_hand_value());
        snprintf(player_hand_label.txt, sizeof(player_hand_label.txt)/sizeof(char), "%d, PUSH.", get_player_hand_value());
        if (get_prev_game_state()==BETTING){
            snprintf(player_money_label.txt, sizeof(player_money_label.txt)/sizeof(char), "$%.2lf", get_player_money());
        }
    }
}

static void update_player_money_if_none_bust(void){
    if (get_prev_game_state()==BETTING){
        if (get_dealer_hand_value() < get_player_hand_value()){
            add_to_player_money(STANDARD_BET_PAYOUT*get_player_bet());
        }
        else if (get_dealer_hand_value() == get_player_hand_value()){
            add_to_player_money(get_player_bet());
        }
    }
}

static void write_labels_if_dealer_bust(void){
    snprintf(dealer_hand_label.txt, sizeof(dealer_hand_label.txt)/sizeof(char), "%d, BUST!", get_dealer_hand_value());
    snprintf(player_hand_label.txt, sizeof(player_hand_label.txt)/sizeof(char), "%d, PLAYER WINS!", get_player_hand_value());
    if (get_prev_game_state()==BETTING){
        snprintf(player_money_label.txt, sizeof(player_money_label.txt)/sizeof(char), "$%.2lf", get_player_money());
    }
}

void on_stand_released(Button *self){
    set_state(&hit_button, DISABLED);
    hit_button.obj.visible = false;
    set_state(&stand_button, DISABLED);
    stand_button.obj.visible = false;
    if (get_prev_game_state()==BETTING){
        set_state(&double_down_button, DISABLED);
        double_down_button.obj.visible = false;
        set_state(&insurance_button, DISABLED);
        insurance_button.obj.visible=false;
    }
    reveal_second_dealer_card();
    dealer_hand_label.obj.visible = (get_dealer_cards_in_hand()>0 && !is_second_dealer_card_hidden()) ? true : false;
    if (dealer_blackjack()){
        snprintf(dealer_hand_label.txt, sizeof(dealer_hand_label.txt)/sizeof(char), "DEALER BLACKJACK!");
        new_game();
    }
    else {
        while (get_dealer_hand_value() < 17){
            dealer_draw_one();
            snprintf(dealer_hand_label.txt, sizeof(dealer_hand_label.txt)/sizeof(char), "%d", get_dealer_hand_value());
        }
        if (!dealer_bust()){
            update_player_money_if_none_bust();
            write_labels_if_none_bust();
        }
        else {
            if (get_prev_game_state()==BETTING){
                add_to_player_money(STANDARD_BET_PAYOUT*get_player_bet());
            }
            write_labels_if_dealer_bust();
        }
        new_game();
        update_label_dimensions(&player_hand_label);
        update_label_dimensions(&player_money_label);
    }
    update_dealer_hand_label_geometry();
}

static void disable_and_hide_valued_buttons_from_player_money(void){
    for (int i=0; i<NUMBER_OF_VALUED_BUTTONS; i++){
        if (get_player_money() < valued_buttons[i]->val){
            set_state(&(valued_buttons[i]->button), DISABLED);
            valued_buttons[i]->button.obj.visible=false;
        }
        else {
            set_state(&(valued_buttons[i]->button), IDLE);
            valued_buttons[i]->button.obj.visible=true;
        }
    }
}

static void reset_player_bet_label(void){
    snprintf(player_bet_label.txt, sizeof(player_bet_label.txt)/sizeof(char), "$0.00");
    update_label_dimensions(&player_bet_label);
    player_bet_label.obj.y = PLAYER_BET_LABEL_Y;
    align_label_x(&player_bet_label, WINDOW_WIDTH/2.f);
    player_bet_label.obj.visible=true;
}

static void disable_player_hand_label(void){
    player_hand_label.txt[0]=0;
    update_label_dimensions(&player_hand_label);
    player_hand_label.obj.visible=false;
}

static void disable_dealer_hand_label(void){
    dealer_hand_label.txt[0]=0;
    update_label_dimensions(&dealer_hand_label);
    player_hand_label.obj.visible=false;
}

void on_bet_released(Button *self){
    set_game_state(BETTING);
    reset_game_state();
    set_state(&bet_button, DISABLED);
    bet_button.obj.visible=false;
    disable_and_hide_valued_buttons_from_player_money();
    reset_player_bet_label();
    disable_player_hand_label();
    disable_dealer_hand_label();
}

void on_stack_released(Button *self){
    set_state(&stack_button, IDLE);
    set_state(&deal_button, DISABLED);

    CHIP_VALUE val = pop_bet();
    add_to_player_money(val);

    ValuedButton *associated_popped_val_button = get_valued_button_associated_to_value(val);
    Chip *chip = create_chip(
        stack_button.obj.x,
        stack_button.obj.y,
        stack_button.obj.width,
        stack_button.obj.height,
        true,
        NULL,
        val
    );
    if (chip==NULL){return;}

    if (peek_bet()==_NONE_CHIP_VALUATION){
        set_state(&stack_button, DISABLED); 
        stack_button.obj.visible=false;
        stack_button.spritesheet=NULL;
    }
    else{
        ValuedButton *associated_peeked_val_button = get_valued_button_associated_to_value(peek_bet());
        stack_button.spritesheet=associated_peeked_val_button->button.spritesheet;
    }

    Animation anim = create_anim(
        &(chip->tex_obj.obj),
        chip->tex_obj.obj.x,
        chip->tex_obj.obj.y,
        0,
        0,
        return_anim
    );
    chip->tex_obj.texture=associated_popped_val_button->button.spritesheet;
    anim.dst_x=associated_popped_val_button->button.obj.x;
    anim.dst_y=associated_popped_val_button->button.obj.y;

    snprintf(player_bet_label.txt, sizeof(player_bet_label.txt)/sizeof(char), "$%.2lf", get_player_bet());
    update_label_dimensions(&player_bet_label);
    align_label_x(&player_bet_label, WINDOW_WIDTH/2.f);

    add_anim(anim);
}

void on_valued_released(Button *button){
    set_state(button, IDLE);
    set_state(&deal_button, DISABLED);
    ValuedButton *val_button = (ValuedButton *)button;
    
    sub_from_player_money(val_button->val);
    push_bet(val_button->val);

    Chip *chip = create_chip(
        button->obj.x, 
        button->obj.y, 
        button->obj.width, 
        button->obj.height, 
        true, 
        button->spritesheet,
        val_button->val
    );
    if (chip==NULL) {return;}
    
    Animation anim = create_anim(
        &(chip->tex_obj.obj),
        chip->tex_obj.obj.x,
        chip->tex_obj.obj.y,
        STACK_BUTTON_X_ORIGIN,
        STACK_BUTTON_Y_ORIGIN,
        bet_anim
    );
    
    disable_and_hide_valued_buttons_from_player_money();
    snprintf(player_money_label.txt, sizeof(player_money_label.txt)/sizeof(char), "$%.2lf", get_player_money());
    update_label_dimensions(&player_money_label);
    
    add_anim(anim);
}

void on_insurance_released(Button *self){
    set_state(&insurance_button, DISABLED);
    insurance_button.obj.visible=false;
    set_state(&double_down_button, DISABLED);
    double_down_button.obj.visible=false;

    sub_from_player_money(get_player_bet()*0.5f);
    snprintf(player_money_label.txt, sizeof(player_money_label.txt)/sizeof(char), "$%.2lf", get_player_money());
    update_label_dimensions(&player_money_label);

    if (dealer_blackjack()){
        reveal_second_dealer_card();
        dealer_hand_label.obj.visible = (get_dealer_cards_in_hand()>0 && !is_second_dealer_card_hidden()) ? true : false;
        add_to_player_money(get_player_bet());
        snprintf(player_money_label.txt, sizeof(player_money_label.txt)/sizeof(char), "$%.2lf", get_player_money());
        update_label_dimensions(&player_money_label);
        new_game();
    }
}

void on_double_down_released(Button *self){
    TexturedGameObject *tex_obj = create_textured_game_object(
        WINDOW_WIDTH - (stack_button.obj.width * 2.5f) - 20,
        WINDOW_HEIGHT/2.f - (stack_button.obj.height)/2.f,
        stack_button.obj.width,
        stack_button.obj.height,
        true,
        stack_button.spritesheet
    );
    if (tex_obj==NULL){return;}
    set_state(&double_down_button, DISABLED);
    double_down_button.obj.visible=false;
    set_state(&insurance_button, DISABLED);
    insurance_button.obj.visible=false;
    
    sub_from_player_money(get_player_bet());
    snprintf(player_money_label.txt, sizeof(player_money_label.txt)/sizeof(char), "$%.2lf", get_player_money());
    update_label_dimensions(&player_money_label);

    snprintf(player_bet_label.txt, sizeof(player_bet_label.txt)/sizeof(char), "2X$%.2lf", get_player_bet());
    update_label_dimensions(&player_bet_label);
    align_label_x(&player_bet_label, (WINDOW_WIDTH - (stack_button.obj.width*1.5f) - 10));
    add_to_player_bet(get_player_bet());
}