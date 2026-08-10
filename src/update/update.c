#include <SDL3/SDL.h>
#include <SDL3/SDL_stdinc.h>

#include "../../include/stb_ds.h"
#include "../../include/vec2.h"
#include "../../include/constants.h"
#include "../../include/game/card_constants.h"
#include "../../include/game/game.h"
#include "../../include/event/event.h"
#include "../../include/ui/button_constants.h"
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

void handle_button_release_deal(GameContext *game_ctx, AnimationQueue *anim_queue, EventQueue *event_queue){
    if (game_context_get_game_state(game_ctx) != GAME_STATE_BETTING) {
        game_reset(game_ctx);
    }
    game_context_set_game_state(game_ctx, GAME_STATE_PLAYING);

    deck_shuffle(game_ctx->deck, game_ctx->deck_top_index_ptr);

    Card* dc1 = dealer_hit(game_ctx->deck, game_ctx->deck_top_index_ptr, game_ctx->dealer);
    anim_enqueue(anim_queue, animation_create(
        &(dc1->rect), 
        vec2_create(HAND_ORIGIN_X, HAND_ORIGIN_Y_DEALER),
        animation_draw_card
    ));
    Card* dc2 = dealer_hit(game_ctx->deck, game_ctx->deck_top_index_ptr, game_ctx->dealer);
    anim_enqueue(anim_queue, animation_create(
        &(dc2->rect), 
        vec2_create((HAND_ORIGIN_X + HAND_STEP_X), HAND_ORIGIN_Y_DEALER),
        animation_draw_card
    ));

    Card *pc1 = player_hit(game_ctx->deck, game_ctx->deck_top_index_ptr, game_ctx->player);
    anim_enqueue(anim_queue, animation_create(
        &(pc1->rect), 
        vec2_create(HAND_ORIGIN_X, HAND_ORIGIN_Y_PLAYER),
        animation_draw_card
    ));
    Card *pc2 = player_hit(game_ctx->deck, game_ctx->deck_top_index_ptr, game_ctx->player);
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

void handle_button_release_hit(GameContext *game_ctx, AnimationQueue *anim_queue, EventQueue *event_queue){
    Card *pc = player_hit(game_ctx->deck, game_ctx->deck_top_index_ptr, game_ctx->player);
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

void handle_button_release_stand(GameContext *game_ctx, AnimationQueue *anim_queue, EventQueue *event_queue){
    dealer_reveal_second_card(game_ctx->dealer);
    while (game_ctx->dealer->hand_value < 17){
        Card *dc = dealer_hit(game_ctx->deck, game_ctx->deck_top_index_ptr, game_ctx->dealer);
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

void handle_button_release_stack(GameContext *game_ctx, AnimationPool *anim_pool, EventQueue *event_queue){
    int cheque_idx = game_ctx->cheque_ring_buffer->tail;
    CHEQUE_VALUE val = player_bet_pop(game_ctx->player);
    CHEQUE_VALUE below_val = player_bet_peek(game_ctx->player);
    TEXTURE_ID stack_tid = (below_val == _CHEQUE_VALUE_NONE) ? TEXTURE_ID_NULL : hmget(game_ctx->cheque_data_map, below_val).cheque_button_tid;
    cheque_ring_buffer_enqueue(
        game_ctx->cheque_ring_buffer, 
        (Cheque){
            {
                {
                    STACK_BUTTON_ORIGIN_X, 
                    STACK_BUTTON_ORIGIN_Y
                },
                CHIP_BUTTON_WIDTH,
                CHIP_BUTTON_HEIGHT,
                true
            },
            val,
            hmget(game_ctx->cheque_data_map, val).cheque_tid,
            true
        }
    );
    anim_add(anim_pool, animation_create(
        &(game_ctx->cheque_ring_buffer->arr[cheque_idx].rect), 
        (vec2){
            .x = hmget(game_ctx->cheque_data_map, val).cheque_button_x, 
            .y = hmget(game_ctx->cheque_data_map, val).cheque_button_y
        },
        animation_cheque_move
    ));
    event_enqueue(
        event_queue, 
        (Event){
            .state={
                .type=STATE_EVENT_CHEQUE_POP_SENT, 
                .data={
                    .cheque_pop_sent={
                        game_ctx->player->bet, 
                        stack_tid
                    }
                }
            }
        }
    );
}

void handle_button_release_cheque(GameContext *game_ctx, AnimationPool *anim_pool, EventQueue *event_queue, CHEQUE_VALUE val){
    int cheque_idx = game_ctx->cheque_ring_buffer->tail;
    game_ctx->player->money -= val;
    cheque_ring_buffer_enqueue(
        game_ctx->cheque_ring_buffer,
        (Cheque){
            {
                {
                    hmget(game_ctx->cheque_data_map, val).cheque_button_x,
                    hmget(game_ctx->cheque_data_map, val).cheque_button_y
                },
                CHIP_BUTTON_WIDTH,
                CHIP_BUTTON_HEIGHT,
                true
            },
            val,
            hmget(game_ctx->cheque_data_map, val).cheque_tid,
            false
        }
    );
    anim_add(anim_pool, animation_create(
        &(game_ctx->cheque_ring_buffer->arr[cheque_idx].rect), 
        (vec2){.x=STACK_BUTTON_ORIGIN_X, .y=STACK_BUTTON_ORIGIN_Y},
        animation_cheque_move
    ));
    event_enqueue(
        event_queue, 
        (Event){
            .state={
                .type=STATE_EVENT_CHEQUE_PUSH_SENT, 
                .data={
                    .cheque_push_sent={
                        game_ctx->player->money
                    }
                }
            }
        }
    );
}

void update(AppState *as){
    update_delta_time(&(as->prev_frametime), &(as->delta_time));
    while (!event_queue_empty(as->event_queue)){
        Event event = event_dequeue(as->event_queue);
        switch (event.type){
            case INPUT_EVENT_BUTTON_RELEASE_DEAL:
                handle_button_release_deal(as->game_ctx, as->anim_queue, as->event_queue);
                event_listeners_notify_all(as->p_event_listeners, event, NULL);
                break;
            case INPUT_EVENT_BUTTON_RELEASE_HIT:
                handle_button_release_hit(as->game_ctx, as->anim_queue, as->event_queue);
                event_listeners_notify_all(as->p_event_listeners, event, NULL);
                break;
            case INPUT_EVENT_BUTTON_RELEASE_STAND:
                handle_button_release_stand(as->game_ctx, as->anim_queue, as->event_queue);
                event_listeners_notify_all(as->p_event_listeners, event, NULL);
                break;
            case INPUT_EVENT_BUTTON_RELEASE_BET:
                game_reset(as->game_ctx);
                game_context_set_game_state(as->game_ctx, GAME_STATE_BETTING);
                event_enqueue(as->event_queue, (Event){.state={.type=STATE_EVENT_BET}});
                event_listeners_notify_all(as->p_event_listeners, event, NULL);
                break;
            case INPUT_EVENT_BUTTON_RELEASE_STACK: {
                handle_button_release_stack(as->game_ctx, as->anim_pool, as->event_queue);
                event_listeners_notify_all(as->p_event_listeners, event, NULL);
                break;
            }
            case INPUT_EVENT_BUTTON_RELEASE_WHITE: {
                handle_button_release_cheque(as->game_ctx, as->anim_pool, as->event_queue, CHEQUE_VALUE_ONE);
                event_listeners_notify_all(as->p_event_listeners, event, NULL);
                break;
            }
            case STATE_EVENT_DEAL: 
            case STATE_EVENT_HIT: 
            case STATE_EVENT_STAND:
            case STATE_EVENT_BET:
            case STATE_EVENT_CHEQUE_PUSH_SENT: 
            case STATE_EVENT_CHEQUE_POP_SENT: {
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
    animate(as->anim_queue, as->anim_pool, as->event_queue, as->delta_time);
    while (!event_queue_empty(as->event_queue)){
        Event event = event_dequeue(as->event_queue);
        switch(event.type){
            case ANIMATION_EVENT_ANIMATION_CARD_DRAW_COMPLETED:
                if (
                    bust(as->game_ctx->player->hand_value) 
                    || blackjack(as->game_ctx->player->cards_in_hand, as->game_ctx->player->hand_value)
                ) {
                    dealer_reveal_second_card(as->game_ctx->dealer);
                }
                flip_card(
                    (Card *)event.anim.target, 
                    is_second_dealer_card(as->game_ctx->dealer, (Card *)event.anim.target),
                    dealer_is_hiding_second_card(as->game_ctx->dealer)
                );
                break;
            case ANIMATION_EVENT_ANIMATION_CHEQUE_COMPLETED: {
                Cheque cheque = cheque_ring_buffer_dequeue(as->game_ctx->cheque_ring_buffer);
                if (!cheque.popped){
                    player_bet_push(as->game_ctx->player, cheque.val);
                    event_enqueue(as->event_queue, (Event){
                        .state={
                            .type=STATE_EVENT_CHEQUE_PUSH_RECEIVED, 
                            .data={
                                .cheque_push_received={
                                    as->game_ctx->player->bet,
                                    hmget(as->game_ctx->cheque_data_map, cheque.val).cheque_button_tid
                                }
                            }
                        }
                    });
                }
                else{
                    as->game_ctx->player->money += cheque.val;
                    event_enqueue(as->event_queue, (Event){
                        .state={
                            .type=STATE_EVENT_CHEQUE_POP_RECEIVED, 
                            .data={
                                .cheque_pop_received=
                                {
                                    as->game_ctx->player->money
                                }
                            }
                        }
                    });
                }
                break;
            }
            case STATE_EVENT_CHEQUE_PUSH_RECEIVED:
            case STATE_EVENT_CHEQUE_POP_RECEIVED: {
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
}