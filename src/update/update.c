#include <SDL3/SDL.h>
#include <SDL3/SDL_stdinc.h>

#include "../../include/vec2.h"
#include "../../include/constants.h"
#include "../../include/game/card_constants.h"
#include "../../include/game/game.h"
#include "../../include/event/event.h"
#include "../../include/ui/label.h"
#include "../../include/main.h"

void update_delta_time(Uint64 *previous_frametime, float *delta_time){
    float time_to_wait = TARGET_FRAME_TIME - (SDL_GetTicks() - *previous_frametime);
    if (time_to_wait > 0 && time_to_wait < TARGET_FRAME_TIME){
        SDL_Delay(time_to_wait);
    }
    *delta_time = (SDL_GetTicks() - *previous_frametime)/1000.f;
    *previous_frametime = SDL_GetTicks();
}

void update_on_new_game(GameContext *game_ctx){
    game_context_set_game_state(game_ctx, GAME_STATE_NEW);
    game_reset(game_ctx);
}

void update_on_button_deal_released(GameContext *game_ctx, AnimationQueue *anim_queue, EventQueue *event_queue){
    game_reset(game_ctx);
    game_context_set_game_state(game_ctx, GAME_STATE_PLAYING);

    deck_shuffle(game_ctx->deck);

    Card* dc1 = dealer_hit(game_ctx->deck, game_ctx->dealer);
    anim_enqueue(anim_queue, animation_create(
        &(dc1->rect), 
        vec2_create(HAND_ORIGIN_X, HAND_ORIGIN_Y_DEALER),
        animation_draw_card
    ));
    Card* dc2 = dealer_hit(game_ctx->deck, game_ctx->dealer);
    anim_enqueue(anim_queue, animation_create(
        &(dc2->rect), 
        vec2_create((HAND_ORIGIN_X + HAND_STEP_X), HAND_ORIGIN_Y_DEALER),
        animation_draw_card
    ));

    Card *pc1 = player_hit(game_ctx->deck, game_ctx->player);
    anim_enqueue(anim_queue, animation_create(
        &(pc1->rect), 
        vec2_create(HAND_ORIGIN_X, HAND_ORIGIN_Y_PLAYER),
        animation_draw_card
    ));
    Card *pc2 = player_hit(game_ctx->deck, game_ctx->player);
    anim_enqueue(anim_queue, animation_create(
        &(pc2->rect), 
        vec2_create((HAND_ORIGIN_X + HAND_STEP_X), HAND_ORIGIN_Y_PLAYER),
        animation_draw_card
    ));

    event_enqueue(event_queue, (Event){.state={
        .type=STATE_EVENT_DEAL,
        .data={
            .hand={
                .dealer_cards_in_hand=game_ctx->dealer->cards_in_hand,
                .dealer_hand_value=game_ctx->dealer->hand_value,
                .player_cards_in_hand=game_ctx->player->cards_in_hand,
                .player_hand_value=game_ctx->player->hand_value
            }
        }
    }});
}

void update_on_button_hit_released(GameContext *game_ctx, AnimationQueue *anim_queue, EventQueue *event_queue){
    Card *pc = player_hit(game_ctx->deck, game_ctx->player);
    anim_enqueue(anim_queue, animation_create(
        &(pc->rect), 
        vec2_create((HAND_ORIGIN_X + HAND_STEP_X*(game_ctx->player->cards_in_hand-1)), HAND_ORIGIN_Y_PLAYER),
        animation_draw_card
    ));
    event_enqueue(event_queue, (Event){.state={
        .type=STATE_EVENT_HIT,
        .data={
            .hand={
                .dealer_cards_in_hand=game_ctx->dealer->cards_in_hand,
                .dealer_hand_value=game_ctx->dealer->hand_value,
                .player_cards_in_hand=game_ctx->player->cards_in_hand,
                .player_hand_value=game_ctx->player->hand_value
            }
        }
    }});
}

void update_on_button_stand_released(GameContext *game_ctx, AnimationQueue *anim_queue, EventQueue *event_queue){
    dealer_reveal_second_card(game_ctx->dealer);
    while (game_ctx->dealer->hand_value < 17){
        Card *dc = dealer_hit(game_ctx->deck, game_ctx->dealer);
        anim_enqueue(anim_queue, animation_create(
            &(dc->rect), 
            vec2_create((HAND_ORIGIN_X + HAND_STEP_X*(game_ctx->dealer->cards_in_hand-1)), HAND_ORIGIN_Y_DEALER),
            animation_draw_card
        ));
    }
    event_enqueue(event_queue, (Event){.state={
        .type=STATE_EVENT_STAND,
        .data={
            .hand={
                .dealer_cards_in_hand=game_ctx->dealer->cards_in_hand,
                .dealer_hand_value=game_ctx->dealer->hand_value,
                .player_cards_in_hand=game_ctx->player->cards_in_hand,
                .player_hand_value=game_ctx->player->hand_value
            }
        }
    }});
}

void update(AppState *as){
    update_delta_time(&(as->prev_frametime), &(as->delta_time));
    /*
    Process input-related events here.
    */
    while (!event_queue_empty(as->event_queue)){
        Event event = event_dequeue(as->event_queue);
        switch (event.type){
            case INPUT_EVENT_BUTTON_RELEASE_DEAL:
                update_on_button_deal_released(as->gctx, as->anim_queue, as->event_queue);
                event_listeners_notify_all(as->p_event_listeners, event, NULL);
                break;
            case INPUT_EVENT_BUTTON_RELEASE_HIT:
                update_on_button_hit_released(as->gctx, as->anim_queue, as->event_queue);
                event_listeners_notify_all(as->p_event_listeners, event, NULL);
                break;
            case INPUT_EVENT_BUTTON_RELEASE_STAND:
                update_on_button_stand_released(as->gctx, as->anim_queue, as->event_queue);
                event_listeners_notify_all(as->p_event_listeners, event, NULL);
                break;
            case STATE_EVENT_DEAL: 
            case STATE_EVENT_HIT: 
            case STATE_EVENT_STAND: {
                label_notify_dependencies label_dependencies = {as->font_map};
                event_listeners_notify_all(as->p_event_listeners, event, (void *)&label_dependencies);
                break;
            }
            default: {
                event_listeners_notify_all(as->p_event_listeners, event, NULL);
                break;
            }
        }
    }
    /*
    Process animation-related events here.
    */
    animate(as->anim_queue, as->event_queue, as->delta_time);
    while (!event_queue_empty(as->event_queue)){
        Event event = event_dequeue(as->event_queue);
        switch(event.type){
            case ANIMATION_EVENT_ANIMATION_CARD_DRAW_COMPLETED:
                if (bust(as->gctx->player->hand_value) || blackjack(as->gctx->player->cards_in_hand, as->gctx->player->hand_value)) {
                    dealer_reveal_second_card(as->gctx->dealer);
                }
                flip_card(
                    (Card *)event.anim.target, 
                    is_second_dealer_card(as->gctx->dealer, (Card *)event.anim.target),
                    dealer_is_hiding_second_card(as->gctx->dealer)
                );
                break;
            default: {
                event_listeners_notify_all(as->p_event_listeners, event, NULL);
                break;
            }
        }
    }
}