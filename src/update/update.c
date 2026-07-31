#include <SDL3/SDL.h>
#include <stdlib.h>
#include "../../include/stb_ds.h"
#include "../../include/constants.h"
#include "../../include/game/card_constants.h"
#include "../../include/game/game.h"
#include "../../include/event/event.h"
#include "../../include/ui/button.h"
#include "../../include/main.h"
#include "../../include/update/update.h"

UpdateContext* update_context_create(void){
    UpdateContext update_ctx = {SDL_GetTicks(), 0.f};
    UpdateContext *p_update_ctx = malloc(sizeof(UpdateContext));
    if (p_update_ctx == NULL){
        abort();
    }
    *p_update_ctx = update_ctx;
    return p_update_ctx;
}

void update_context_destroy(UpdateContext *update_ctx){
    free(update_ctx);
}

void update_delta_time(UpdateContext *update_ctx){
    float time_to_wait = TARGET_FRAME_TIME - (SDL_GetTicks() - update_ctx->previous_frame_time);
    if (time_to_wait > 0 && time_to_wait < TARGET_FRAME_TIME){
        SDL_Delay(time_to_wait);
    }
    update_ctx->delta_time = (SDL_GetTicks() - update_ctx->previous_frame_time)/1000.f;
    update_ctx->previous_frame_time = SDL_GetTicks();
}

void deal(GameContext *game_ctx, EventQueue *queue){
    game_context_set_game_state(game_ctx, GAME_STATE_PLAYING);

    deck_shuffle(game_ctx->deck);

    Card* dc1 = dealer_hit(game_ctx->deck, game_ctx->dealer);
    dc1->obj.pos.x = HAND_ORIGIN_X;
    dc1->obj.pos.y = HAND_ORIGIN_Y_DEALER;
    dc1->face_down = false;
    Card* dc2 = dealer_hit(game_ctx->deck, game_ctx->dealer);
    dc2->obj.pos.x = HAND_ORIGIN_X + CARD_STEP_X;
    dc2->obj.pos.y = HAND_ORIGIN_Y_DEALER;
    dc2->face_down = false;

    Card *pc1 = player_hit(game_ctx->deck, game_ctx->player);
    pc1->obj.pos.x = HAND_ORIGIN_X;
    pc1->obj.pos.y = HAND_ORIGIN_Y_PLAYER;
    pc1->face_down = false;
    Card *pc2 = player_hit(game_ctx->deck, game_ctx->player);
    pc2->obj.pos.x = HAND_ORIGIN_X + CARD_STEP_X;
    pc2->obj.pos.y = HAND_ORIGIN_Y_PLAYER;
    pc2->face_down = false;

    if (dealer_is_blackjack(game_ctx->dealer)){
        
    }
}

void hit(GameContext *game_ctx){
}

void update(AppState *as){
    update_delta_time(as->update_ctx);
    while (!event_queue_empty(as->event_ctx->queue)){
        Event event = event_dequeue(as->event_ctx->queue);
        switch (event.type){
            case BUTTON_EVENT_RELEASE_DEAL:
                deal(as->game_ctx, as->event_ctx->queue);
                break;
            case BUTTON_EVENT_RELEASE_HIT:
                hit(as->game_ctx);
                break;
            default:
                break;
        }
        event_ctx_widget_listeners_notify_all(as->event_ctx, event);
    }
    button_ctx_reposition_visible_move_buttons(as->ui_ctx->button_ctx); // enqueue an event to run this
    // after a button_release event
    //animate_from_queue(p_ac, p_ec, p_uc);
}