#include <SDL3/SDL.h>
#include <stdlib.h>
#include <stdio.h>
#include "../include/stb_ds.h"
#include "../include/constants.h"
#include "../include/button.h"
#include "../include/update.h"
#include "../include/game.h"

UpdateContext* update_context_create(void){
    UpdateContext uc = {SDL_GetTicks(), 0.f};
    UpdateContext *p_uc = malloc(sizeof(UpdateContext));
    if (p_uc == NULL){
        abort();
    }
    *p_uc = uc;
    return p_uc;
}

void update_context_destroy(UpdateContext *p_uc){
    free(p_uc);
    p_uc = NULL;
}

void update_delta_time(UpdateContext *p_uc){
    float time_to_wait = TARGET_FRAME_TIME - (SDL_GetTicks() - p_uc->previous_frame_time);
    if (time_to_wait > 0 && time_to_wait < TARGET_FRAME_TIME){
        SDL_Delay(time_to_wait);
    }
    p_uc->delta_time = (SDL_GetTicks() - p_uc->previous_frame_time)/1000.f;
    p_uc->previous_frame_time = SDL_GetTicks();
}

void update_game_on_release_deal(GameContext *gc){
    printf("i, deal, am released!\n");
}

void update_game_on_release_hit(GameContext *gc){
    printf("i, hit, am released!\n");
}

void update(UpdateContext *p_uc, GameContext *p_gc, EventContext *p_ec, ButtonContext *p_bc){
    update_delta_time(p_uc);
    while (!event_queue_empty(p_ec->queue)){
        Event event = dequeue_event(p_ec->queue);
        switch (event.type){
            case BUTTON_EVENT_RELEASE_DEAL:
                update_game_on_release_deal(p_gc);
                break;
            case BUTTON_EVENT_RELEASE_HIT:
                update_game_on_release_hit(p_gc);
                break;
            default:
                break;
        }
        ec_widget_listeners_notify_all(p_ec, event);
    }
    bc_update_move_button_positions_from_visibilities(p_bc);
}