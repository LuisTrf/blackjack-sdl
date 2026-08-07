#include <SDL3/SDL.h>
#include <stdlib.h>
#include <stdio.h>
#include "../../include/stb_ds.h"
#include "../../include/event/event.h"

EventQueue* event_queue_create(int size){
    Event* arr = calloc(size, sizeof(Event));
    if (arr == NULL){
        abort();
    }
    EventQueue queue = {.size=size, .head=0, .tail=0, .arr=arr};
    EventQueue *p_queue = malloc(sizeof(queue));
    if (p_queue == NULL){
        abort();
    }
    *p_queue = queue;
    return p_queue;
}

void event_queue_destroy(EventQueue *queue){
    free(queue->arr);
    queue->arr = NULL;
    free(queue);
}

bool event_queue_full(EventQueue *queue){
    if (
        queue->head==queue->tail+1 
        || (queue->head==0 && queue->tail==queue->size-1)
    ){
        return true;
    }
    else {
        return false;
    }
}

bool event_queue_empty(EventQueue *queue){
    if (queue->head==queue->tail) {
        return true;
    }
    else {
        return false;
    }
}

void event_enqueue(EventQueue *queue, Event event){
    if (event_queue_full(queue)){
        fprintf(stderr, "EVENT QUEUE OVERFLOW!");
        return;
    }
    else{
        queue->arr[queue->tail] = event;
        if (queue->tail==queue->size-1){
            queue->tail=0;
        }
        else {
            queue->tail++;
        }
    }
}

Event event_dequeue(EventQueue *queue){
    if (event_queue_empty(queue)){
        fprintf(stderr, "EVENT QUEUE UNDERFLOW!");
        return NULL_EVENT;
    }
    else {
        Event e = queue->arr[queue->head];
        if (queue->head==queue->size-1){
            queue->head=0;
        }
        else {
            queue->head++;
        }
        return e;
    }
}

bool event_is_null(Event event){
    if (event.type == NULL_EVENT.type){
        return true;
    }
    else {
        return false;
    }
}

void event_listeners_notify_all(EventListener **event_listeners, Event event, void *dependencies){
    for (int i = 0; i < arrlen(*event_listeners); i++){
        (*event_listeners)[i].notify_func((void *)(*event_listeners)[i].self, event, dependencies);
    }
}

void event_listener_register(EventListener **event_listeners, EventListener event_listener){
    arrput(*event_listeners, event_listener);
}

/*
void handle_end_of_draw_anim_event(Event event){
    Card *card = (Card *)event.cargo.game_object;
    card->face_down = (get_dealer_hand()[1]==card && !player_blackjack()) ? true : false;
    dealer_hand_label.obj.visible = (get_dealer_cards_in_hand()>0 && !is_second_dealer_card_hidden()) ? true : false;
    player_hand_label.obj.visible = (get_player_cards_in_hand()>0) ? true : false;
}

void handle_end_of_bet_anim_event(Event event){
    Chip *chip = (Chip *)(TexturedGameObject *)event.cargo.game_object;

    set_state(&stack_button, IDLE);
    stack_button.spritesheet=chip->tex_obj.texture;
    stack_button.obj.visible=true;

    snprintf(player_bet_label.txt, sizeof(player_bet_label.txt)/sizeof(char), "$%.2lf", get_player_bet());
    update_label_dimensions(&player_bet_label);
    align_label_x(&player_bet_label, WINDOW_WIDTH/2.f);

    destroy_chip(chip);

    set_state(&deal_button, IDLE);
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

void handle_end_of_return_anim_event(Event event){
    Chip *chip = (Chip *)(TexturedGameObject *)event.cargo.game_object;

    snprintf(player_money_label.txt, sizeof(player_money_label.txt)/sizeof(char), "$%.2lf", get_player_money());
    update_label_dimensions(&player_money_label);
    disable_and_hide_valued_buttons_from_player_money();

    destroy_chip(chip);

    set_state(&deal_button, IDLE);
}

void handle_disable_buttons_event(Event event){
    disable_all_buttons();
}

void handle_restore_buttons_event(Event event){
    restore_all_button_prev_states();
}

static void disable_player_bet_label(void){
    player_bet_label.txt[0]=0;
    update_label_dimensions(&player_bet_label);
    player_bet_label.obj.visible=false;
}

void handle_animation_queue_finished_event(Event event){
    if (get_game_state()==NEW){
        disable_player_bet_label();
        destroy_all_textured_game_objects();
    }
}

void handle_new_game_event(Event event){
    if (get_anim_queue_state()==QUEUE_IDLE){
        disable_player_bet_label();
        destroy_all_textured_game_objects();
    }
}

void switch_event(Event event){
    switch (event.type){
        case _NONE_EVENT_TYPE:
            return;
        case END_OF_DRAW_ANIM_EVENT:
            handle_end_of_draw_anim_event(event);
            break;
        case END_OF_BET_ANIM_EVENT:
            handle_end_of_bet_anim_event(event);
            break;
        case END_OF_RETURN_ANIM_EVENT:
            handle_end_of_return_anim_event(event);
            break;
        case DISABLE_BUTTONS_EVENT:
            handle_disable_buttons_event(event);
            break;
        case RESTORE_BUTTONS_EVENT:
            handle_restore_buttons_event(event);
            break;
        case ANIMATION_QUEUE_FINISHED_EVENT:
            handle_animation_queue_finished_event(event);
            break;
        case NEW_GAME_EVENT:
            handle_new_game_event(event);
            break;
    }
}

void poll_events(void){
    while (!event_queue_empty()){
        Event event = dequeue_event();
        switch_event(event);
    }
}
*/