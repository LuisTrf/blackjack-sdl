#include <SDL3/SDL.h>
#include <stdlib.h>
#include "../../include/stb_ds.h"
#include "../../include/constants.h"
#include "../../include/ui/button.h"
#include "../../include/game/game.h"
#include "../../include/event/event.h"
#include "../../include/main.h"
#include "../../include/update/update_internal.h"

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

    game_shuffle_deck(gc->deck);

    Card* dc1 = game_dealer_hit(gc->deck, gc->dealer);
    Card* dc2 = game_dealer_hit(gc->deck, gc->dealer);

    Card *pc1 = game_player_hit(gc->deck, gc->player);
    Card *pc2 = game_player_hit(gc->deck, gc->player);

    if (game_dealer_is_blackjack(gc->dealer)){
        
    }
}

void update_game_on_release_hit(Game_Context *gc){
}

void update(App_State *as){
    update_delta_time(p_uc);
    while (!event_queue_empty(p_ec->queue)){
        Event event = dequeue_event(p_ec->queue);
        switch (event.type){
            case BUTTON_EVENT_RELEASE_DEAL:
                deal(p_gc, p_ec->queue);
                break;
            case BUTTON_EVENT_RELEASE_HIT:
                hit(p_gc);
                break;
            default:
                break;
        }
        ec_widget_listeners_notify_all(p_ec, event);
    }
    bc_update_move_button_positions_from_visibilities(p_bc); //replace via event
    animate_from_queue(p_ac, p_ec, p_uc);
}