#include <SDL3/SDL.h>
#include <stdlib.h>

#include "../../include/stb_ds.h"
#include "../../include/vec2.h"
#include "../../include/constants.h"
#include "../../include/game/card_constants.h"
#include "../../include/game/game.h"
#include "../../include/event/event.h"
#include "../../include/ui/label.h"
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
    update_delta_time(as->update_ctx);
    /*
    Process input-related events here.
    */
    while (!event_queue_empty(as->event_ctx->queue)){
        Event event = event_dequeue(as->event_ctx->queue);
        switch (event.type){
            case INPUT_EVENT_BUTTON_RELEASE_DEAL:
                update_on_button_deal_released(as->game_ctx, as->anim_ctx->queue, as->event_ctx->queue);
                event_ctx_listeners_notify_all(as->event_ctx, event, NULL);
                break;
            case INPUT_EVENT_BUTTON_RELEASE_HIT:
                update_on_button_hit_released(as->game_ctx, as->anim_ctx->queue, as->event_ctx->queue);
                event_ctx_listeners_notify_all(as->event_ctx, event, NULL);
                break;
            case INPUT_EVENT_BUTTON_RELEASE_STAND:
                update_on_button_stand_released(as->game_ctx, as->anim_ctx->queue, as->event_ctx->queue);
                event_ctx_listeners_notify_all(as->event_ctx, event, NULL);
                break;
            case STATE_EVENT_DEAL: 
            case STATE_EVENT_HIT: 
            case STATE_EVENT_STAND: {
                label_notify_dependencies label_dependencies = {as->font_map};
                event_ctx_listeners_notify_all(as->event_ctx, event, (void *)&label_dependencies);
                button_ctx_reposition_visible_move_buttons(as->ui_ctx->button_ctx);
                break;
            }
            default: {
                event_ctx_listeners_notify_all(as->event_ctx, event, NULL);
                break;
            }
        }
    }
    /*
    Process animation-related events here.
    */
    animate(as);
    while (!event_queue_empty(as->event_ctx->queue)){
        Event event = event_dequeue(as->event_ctx->queue);
        switch(event.type){
            case ANIMATION_EVENT_ANIMATION_CARD_DRAW_COMPLETED:
                if (bust(as->game_ctx->player->hand_value) || blackjack(as->game_ctx->player->cards_in_hand, as->game_ctx->player->hand_value)) {
                    dealer_reveal_second_card(as->game_ctx->dealer);
                }
                flip_card(
                    (Card *)event.anim.target, 
                    is_second_dealer_card(as->game_ctx->dealer, (Card *)event.anim.target),
                    dealer_is_hiding_second_card(as->game_ctx->dealer)
                );
                break;
            default: {
                event_ctx_listeners_notify_all(as->event_ctx, event, NULL);
                break;
            }
        }
    }
}