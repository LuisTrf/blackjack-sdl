#include <SDL3/SDL.h>
#include <stdlib.h>
#include "../include/stb_ds.h"
#include "../include/constants.h"
#include "../include/button.h"
#include "../include/update_context.h"
#include "../include/game.h"
#include "../include/animation_context.h"

#include <stdio.h>

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

void update_game_on_release_deal(GameContext *gc, EventQueue *queue){
    game_context_set_game_state(gc, GAME_STATE_PLAYING);

    Card* dc1 = game_dealer_hit(gc->deck, gc->dealer);
    Event e1 = {.card={CARD_EVENT_CARD_DRAWN, dc1, gc->dealer->cards_in_hand, gc->player->cards_in_hand}};
    enqueue_event(queue, e1);
    Card* dc2 = game_dealer_hit(gc->deck, gc->dealer);
    Event e2 = {.card={CARD_EVENT_CARD_DRAWN, dc2, gc->dealer->cards_in_hand, gc->player->cards_in_hand}};
    enqueue_event(queue, e2);

    Card *pc1 = game_player_hit(gc->deck, gc->player);
    Event e3 = {.card={CARD_EVENT_CARD_DRAWN, pc1, gc->dealer->cards_in_hand, gc->player->cards_in_hand}};
    enqueue_event(queue, e3);
    Card *pc2 = game_player_hit(gc->deck, gc->player);
    Event e4 = {.card={CARD_EVENT_CARD_DRAWN, pc2, gc->dealer->cards_in_hand, gc->player->cards_in_hand}};
    enqueue_event(queue, e4);

    if (game_dealer_is_blackjack(gc->dealer)){
        Event e5 = {.game={GAME_EVENT_NEW_GAME, gc->dealer, gc->player}};
        enqueue_event(queue, e5);
        game_participants_reset(gc->dealer, gc->player);
    }
}

void update_game_on_release_hit(GameContext *gc){
}

void update(UpdateContext *p_uc, GameContext *p_gc, EventContext *p_ec, AnimationContext *p_ac, ButtonContext *p_bc){
    update_delta_time(p_uc);
    while (!event_queue_empty(p_ec->queue)){
        Event event = dequeue_event(p_ec->queue);
        switch (event.type){
            case BUTTON_EVENT_RELEASE_DEAL:
                update_game_on_release_deal(p_gc, p_ec->queue);
                break;
            case BUTTON_EVENT_RELEASE_HIT:
                update_game_on_release_hit(p_gc);
                break;
            case VEC2ANIM_EVENT_CREATE_NEW:
                printf("queued anim\n");
                enqueue_anim(
                    p_ac->queue,
                    vec2_animation_create(
                        event.v2anim.tgt,
                        event.v2anim.dst, 
                        animation_draw_card
                    )
                );
                /*
                event.v2anim.anim_tgt->x = event.v2anim.dst.x;
                event.v2anim.anim_tgt->y = event.v2anim.dst.y;
                */
            default:
                break;
        }
        ec_widget_listeners_notify_all(p_ec, event);
    }
    bc_update_move_button_positions_from_visibilities(p_bc); //replace via event
    animate_from_queue(p_ac, p_ec, p_uc);
}