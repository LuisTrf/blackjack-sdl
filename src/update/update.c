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

Update_Context* update_context_create(void){
    Update_Context uc = {SDL_GetTicks(), 0.f};
    Update_Context *p_uc = malloc(sizeof(Update_Context));
    if (p_uc == NULL){
        abort();
    }
    *p_uc = uc;
    return p_uc;
}

void update_context_destroy(Update_Context *update_ctx){
    free(update_ctx);
}

void update_delta_time(Update_Context *update_ctx){
    float time_to_wait = TARGET_FRAME_TIME - (SDL_GetTicks() - update_ctx->previous_frame_time);
    if (time_to_wait > 0 && time_to_wait < TARGET_FRAME_TIME){
        SDL_Delay(time_to_wait);
    }
    update_ctx->delta_time = (SDL_GetTicks() - update_ctx->previous_frame_time)/1000.f;
    update_ctx->previous_frame_time = SDL_GetTicks();
}

void deal(Game_Context *game_ctx, EventQueue *queue){
    game_context_set_game_state(game_ctx, GAME_STATE_PLAYING);

    game_shuffle_deck(game_ctx->deck);

    Card* dc1 = game_dealer_hit(game_ctx->deck, game_ctx->dealer);
    dc1->obj.pos.x = HAND_ORIGIN_X;
    dc1->obj.pos.y = HAND_ORIGIN_Y_DEALER;
    dc1->face_down = false;
    Card* dc2 = game_dealer_hit(game_ctx->deck, game_ctx->dealer);
    dc2->obj.pos.x = HAND_ORIGIN_X + CARD_STEP_X;
    dc2->obj.pos.y = HAND_ORIGIN_Y_DEALER;
    dc2->face_down = false;

    Card *pc1 = game_player_hit(game_ctx->deck, game_ctx->player);
    pc1->obj.pos.x = HAND_ORIGIN_X;
    pc1->obj.pos.y = HAND_ORIGIN_Y_PLAYER;
    pc1->face_down = false;
    Card *pc2 = game_player_hit(game_ctx->deck, game_ctx->player);
    pc2->obj.pos.x = HAND_ORIGIN_X + CARD_STEP_X;
    pc2->obj.pos.y = HAND_ORIGIN_Y_PLAYER;
    pc2->face_down = false;

    if (game_dealer_is_blackjack(game_ctx->dealer)){
        
    }
}

void hit(Game_Context *game_ctx){
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
    bc_update_move_button_positions_from_visibilities(as->uic->bc); // enqueue an event to run this
    // after a button_release event, also rename this method
    //animate_from_queue(p_ac, p_ec, p_uc);
}