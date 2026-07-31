#include <SDL3/SDL.h>
#include <stdlib.h>
#include "../../include/stb_ds.h"
#include "../../include/constants.h"
#include "../../include/game/game.h"
#include "../../include/event/event.h"
#include "../../include/main.h"
#include "../../include/update/update.h"

#include <stdio.h>

Update_Context* update_context_create(void){
    Update_Context uc = {SDL_GetTicks(), 0.f};
    Update_Context *p_uc = malloc(sizeof(Update_Context));
    if (p_uc == NULL){
        abort();
    }
    *p_uc = uc;
    return p_uc;
}

void update_context_destroy(Update_Context *p_uc){
    free(p_uc);
    p_uc = NULL;
}

void update_delta_time(Update_Context *p_uc){
    float time_to_wait = TARGET_FRAME_TIME - (SDL_GetTicks() - p_uc->previous_frame_time);
    if (time_to_wait > 0 && time_to_wait < TARGET_FRAME_TIME){
        SDL_Delay(time_to_wait);
    }
    p_uc->delta_time = (SDL_GetTicks() - p_uc->previous_frame_time)/1000.f;
    p_uc->previous_frame_time = SDL_GetTicks();
}

void deal(Game_Context *gc, EventQueue *queue){
    game_context_set_game_state(gc, GAME_STATE_PLAYING);

    Deck* deck = gc_get_deck(gc);
    Dealer* dealer = gc_get_dealer(gc);
    Player* player = gc_get_player(gc);

    game_shuffle_deck(deck);

    Card* dc1 = game_dealer_hit(deck, dealer);
    Card* dc2 = game_dealer_hit(deck, dealer);

    Card *pc1 = game_player_hit(deck, player);
    Card *pc2 = game_player_hit(deck, player);

    if (game_dealer_is_blackjack(dealer)){
        
    }
}

void hit(Game_Context *gc){
}

void update(App_State *as){
    update_delta_time(as->uc);
    while (!event_queue_empty(as->ec->queue)){
        Event event = dequeue_event(as->ec->queue);
        switch (event.type){
            case BUTTON_EVENT_RELEASE_DEAL:
                deal(as->gc, as->ec->queue);
                break;
            case BUTTON_EVENT_RELEASE_HIT:
                hit(as->gc);
                break;
            default:
                break;
        }
        ec_widget_listeners_notify_all(as->ec, event);
    }
    bc_update_move_button_positions_from_visibilities(as->uic->bc);
    //animate_from_queue(p_ac, p_ec, p_uc);
}