#include <SDL3/SDL.h>
#include <SDL3/SDL_stdinc.h>

#include "../../include/stb_ds.h"
#include "../../include/vec2.h"
#include "../../include/constants.h"
#include "../../include/game/card_constants.h"
#include "../../include/game/game.h"
#include "../../include/event/event.h"
#include "../../include/ui/button_constants.h"
#include "../../include/main.h"

#include <stdio.h>

void update_delta_time(Uint64 *previous_frametime, float *delta_time){
    float time_to_wait = TARGET_FRAME_TIME - (SDL_GetTicks() - *previous_frametime);
    if (time_to_wait > 0 && time_to_wait < TARGET_FRAME_TIME){
        SDL_Delay(time_to_wait);
    }
    *delta_time = (SDL_GetTicks() - *previous_frametime)/1000.f;
    *previous_frametime = SDL_GetTicks();
}

void queue_game_state_event(GameContext *game_ctx, EventQueue *event_queue){
    event_enqueue(event_queue, (Event){
        .state={
            .type=STATE_EVENT_GAME_STATE,
            .data={
                .game_state={
                    .game_state=game_context_get_game_state(game_ctx),
                    .prev_game_state=game_context_get_prev_game_state(game_ctx)
                }
            }
        }
    });
}

void queue_bet_payout_event(EventQueue *event_queue, float money){
    event_enqueue(event_queue, (Event){
        .state={
            .type=STATE_EVENT_BET_PAYOUT,
            .data={
                .bet_payout = {money}
            }
        }
    });
}

void queue_card_animation(AnimationQueue *anim_queue, Card *tgt_card, vec2 dst){
    anim_enqueue(anim_queue, (Animation){
        &(tgt_card->rect),
        ANIMATION_TYPE_VEC2,
        ANIMATION_STATE_WAITING,
        animation_draw_card,
        {.vec2_anim={
            (vec2){tgt_card->rect.pos.x, tgt_card->rect.pos.y},
            dst
        }}
    });
}

void handle_button_release_deal(GameContext *game_ctx, AnimationQueue *anim_queue, EventQueue *event_queue){
    if (game_context_get_game_state(game_ctx) == GAME_STATE_BETTING) {
        game_context_set_game_state(game_ctx, GAME_STATE_BETTING_PLAYING);
    }
    else{
        game_reset(game_ctx);
        game_context_set_game_state(game_ctx, GAME_STATE_PLAYING);
    }
    queue_game_state_event(game_ctx, event_queue);

    deck_shuffle(game_ctx->deck, game_ctx->deck_top_index_ptr);

    Card* dc1 = dealer_hit(game_ctx->deck, game_ctx->deck_top_index_ptr, game_ctx->dealer);
    queue_card_animation(anim_queue, dc1, (vec2){HAND_ORIGIN_X, HAND_ORIGIN_Y_DEALER});
    Card* dc2 = dealer_hit(game_ctx->deck, game_ctx->deck_top_index_ptr, game_ctx->dealer);
    queue_card_animation(anim_queue, dc2, (vec2){(HAND_ORIGIN_X + HAND_STEP_X), HAND_ORIGIN_Y_DEALER});

    Card *pc1 = player_hit(game_ctx->deck, game_ctx->deck_top_index_ptr, game_ctx->player);
    queue_card_animation(anim_queue, pc1, (vec2){HAND_ORIGIN_X, HAND_ORIGIN_Y_PLAYER});
    Card *pc2 = player_hit(game_ctx->deck, game_ctx->deck_top_index_ptr, game_ctx->player);
    queue_card_animation(anim_queue, pc2, (vec2){(HAND_ORIGIN_X + HAND_STEP_X), HAND_ORIGIN_Y_PLAYER});

    if (
        game_context_get_game_state(game_ctx) == GAME_STATE_BETTING_PLAYING
        && blackjack(game_ctx->player->cards_in_hand, game_ctx->player->hand_value)
    ){
        if (!blackjack(game_ctx->dealer->cards_in_hand, game_ctx->dealer->hand_value)){
            game_ctx->player->money += game_ctx->player->bet * PLAYER_BLACKJACK_BET_PAYOUT;
            queue_bet_payout_event(event_queue, game_ctx->player->money);
        }
        else {
            game_ctx->player->money += game_ctx->player->bet;
            queue_bet_payout_event(event_queue, game_ctx->player->money);
        }
    }

    if (
        pc1->rank_value == pc2->rank_value
        && (game_context_get_game_state(game_ctx) == GAME_STATE_BETTING_PLAYING)
        && game_ctx->player->money >= game_ctx->player->bet
    ){
        event_enqueue(event_queue, (Event){.state={.type=STATE_EVENT_SPLIT_POSSIBLE}});
    }

    event_enqueue(event_queue, (Event){.state={
        .type=STATE_EVENT_DEAL,
        .data={
            .deal={
                .dealer_cards_in_hand=game_ctx->dealer->cards_in_hand,
                .dealer_hand_value=game_ctx->dealer->hand_value,
                .player_cards_in_hand=game_ctx->player->cards_in_hand,
                .player_hand_value=game_ctx->player->hand_value
            }
        }
    }});
}

void handle_button_release_hit(GameContext *game_ctx, AnimationQueue *anim_queue, EventQueue *event_queue){
    GAME_STATE game_state = game_context_get_game_state(game_ctx);
    float card_y = 0;
    switch (game_state){
        case GAME_STATE_PLAYING:
            card_y = HAND_ORIGIN_Y_PLAYER;
            break;
        case GAME_STATE_BETTING_PLAYING:
            card_y = (game_ctx->player->cards_in_split_hand > 0) ? HAND_SPLITTING_Y_PLAYER : HAND_ORIGIN_Y_PLAYER;
            break;
        case GAME_STATE_PLAYING_SPLIT:
            card_y = SPLIT_HAND_SPLITTING_Y_PLAYER;
            break;
        default:
            break;
    }
    Card *pc = (game_state == GAME_STATE_PLAYING_SPLIT) ? 
        player_hit_split(game_ctx->deck, game_ctx->deck_top_index_ptr, game_ctx->player) :
        player_hit(game_ctx->deck, game_ctx->deck_top_index_ptr, game_ctx->player);
    float card_x = 0;
    switch (game_state){
        case GAME_STATE_PLAYING_SPLIT: 
            card_x = HAND_ORIGIN_X + HAND_STEP_X*(game_ctx->player->cards_in_split_hand - 1);
            break;
        case GAME_STATE_BETTING_PLAYING:
        case GAME_STATE_PLAYING:
            card_x = HAND_ORIGIN_X + HAND_STEP_X*(game_ctx->player->cards_in_hand - 1);
            break;
        default:
            break;
    }
    queue_card_animation(anim_queue, pc, (vec2){card_x, card_y});
    if (game_state == GAME_STATE_PLAYING_SPLIT){
        event_enqueue(event_queue, (Event){.state={
            .type=STATE_EVENT_SPLIT_HIT,
            .data={
                .split_hit={
                    .player_cards_in_split_hand = game_ctx->player->cards_in_split_hand,
                    .player_split_hand_value = game_ctx->player->split_hand_value,
                    .bet = game_ctx->player->bet
                }
            }
        }});
        if (bust(game_ctx->player->split_hand_value)){
            game_context_set_game_state(game_ctx, GAME_STATE_BETTING_PLAYING);
            queue_game_state_event(game_ctx, event_queue);
        }
    }
    else {
        event_enqueue(event_queue, (Event){.state={
            .type=STATE_EVENT_HIT,
            .data={
                .hit={
                    .dealer_cards_in_hand=game_ctx->dealer->cards_in_hand,
                    .dealer_hand_value=game_ctx->dealer->hand_value,
                    .player_cards_in_hand=game_ctx->player->cards_in_hand,
                    .player_hand_value=game_ctx->player->hand_value,
                    .player_cards_in_split_hand=game_ctx->player->cards_in_split_hand,
                    .player_split_hand_value=game_ctx->player->split_hand_value
                }
            }
        }});
        if (bust(game_ctx->player->hand_value)){
            dealer_reveal_second_card(game_ctx->dealer);
            if (game_context_get_prev_game_state(game_ctx) == GAME_STATE_PLAYING_SPLIT){
                if (
                    !bust(game_ctx->player->split_hand_value)
                    && !blackjack(game_ctx->dealer->cards_in_hand, game_ctx->dealer->hand_value)
                ){
                    if (
                        game_ctx->player->split_hand_value > game_ctx->dealer->hand_value
                        || bust(game_ctx->dealer->hand_value)
                    ){
                        game_ctx->player->money += game_ctx->player->split_bet * STANDARD_BET_PAYOUT;
                        queue_bet_payout_event(event_queue, game_ctx->player->money);
                    }
                    else if (
                        game_ctx->player->hand_value == game_ctx->dealer->hand_value
                        && !blackjack(game_ctx->dealer->cards_in_hand, game_ctx->dealer->hand_value)
                    ){
                        game_ctx->player->money += game_ctx->player->split_bet;
                        queue_bet_payout_event(event_queue, game_ctx->player->money);
                    }
                }
            }
            game_context_set_game_state(game_ctx, GAME_STATE_FIN);
            queue_game_state_event(game_ctx, event_queue);
        }
    }
}

void handle_button_release_stand(GameContext *game_ctx, AnimationQueue *anim_queue, EventQueue *event_queue){
    if (game_context_get_game_state(game_ctx) == GAME_STATE_PLAYING_SPLIT){
        game_context_set_game_state(game_ctx, GAME_STATE_BETTING_PLAYING);
        queue_game_state_event(game_ctx, event_queue);
        return;
    }
    dealer_reveal_second_card(game_ctx->dealer);
    float dealer_hand_y = (game_context_get_prev_game_state(game_ctx) == GAME_STATE_PLAYING_SPLIT) ? 
        HAND_SPLITTING_Y_DEALER :
        HAND_ORIGIN_Y_DEALER;
    while (game_ctx->dealer->hand_value < 17){
        Card *dc = dealer_hit(game_ctx->deck, game_ctx->deck_top_index_ptr, game_ctx->dealer);
        queue_card_animation(anim_queue, dc, (vec2){(HAND_ORIGIN_X + HAND_STEP_X*(game_ctx->dealer->cards_in_hand-1)), dealer_hand_y});
    }

    if (game_context_get_game_state(game_ctx) == GAME_STATE_BETTING_PLAYING){
        if (
            !bust(game_ctx->player->hand_value)
            && !blackjack(game_ctx->dealer->cards_in_hand, game_ctx->dealer->hand_value)
        ){
            if (
                game_ctx->player->hand_value > game_ctx->dealer->hand_value 
                || bust(game_ctx->dealer->hand_value)
            ){
                game_ctx->player->money += game_ctx->player->bet * STANDARD_BET_PAYOUT;
                queue_bet_payout_event(event_queue, game_ctx->player->money);
            }
            else if (
                game_ctx->player->hand_value == game_ctx->dealer->hand_value 
                && !blackjack(game_ctx->dealer->cards_in_hand, game_ctx->dealer->hand_value)
            ){
                game_ctx->player->money += game_ctx->player->bet;
                queue_bet_payout_event(event_queue, game_ctx->player->money);
            }
        }
    }
    if (game_context_get_prev_game_state(game_ctx) == GAME_STATE_PLAYING_SPLIT){
        if (
            !bust(game_ctx->player->split_hand_value)
            && !blackjack(game_ctx->dealer->cards_in_hand, game_ctx->dealer->hand_value)
        ){
            if (
                game_ctx->player->split_hand_value > game_ctx->dealer->hand_value
                || bust(game_ctx->dealer->hand_value)
            ){
                game_ctx->player->money += game_ctx->player->split_bet * STANDARD_BET_PAYOUT;
                queue_bet_payout_event(event_queue, game_ctx->player->money);
            }
            else if (
                game_ctx->player->hand_value == game_ctx->dealer->hand_value
                && !blackjack(game_ctx->dealer->cards_in_hand, game_ctx->dealer->hand_value)
            ){
                game_ctx->player->money += game_ctx->player->split_bet;
                queue_bet_payout_event(event_queue, game_ctx->player->money);
            }
        }
    }
    event_enqueue(event_queue, (Event){
        .state={
            .type=STATE_EVENT_STAND,
            .data={
                .stand={
                    .dealer_cards_in_hand=game_ctx->dealer->cards_in_hand,
                    .dealer_hand_value=game_ctx->dealer->hand_value,
                    .player_cards_in_hand=game_ctx->player->cards_in_hand,
                    .player_hand_value=game_ctx->player->hand_value,
                    .player_cards_in_split_hand=game_ctx->player->cards_in_split_hand,
                    .player_split_hand_value=game_ctx->player->split_hand_value
                }
            }
        }
    });
    game_context_set_game_state(game_ctx, GAME_STATE_FIN);
    queue_game_state_event(game_ctx, event_queue);
}

void handle_button_release_bet(GameContext *game_ctx, EventQueue *event_queue){
    game_reset(game_ctx);
    game_context_set_game_state(game_ctx, GAME_STATE_BETTING);
    event_enqueue(event_queue, (Event){
        .state={
            .type=STATE_EVENT_BET,
            .data={
                .bet={
                    .money = game_ctx->player->money
                }
            }
        }
    });
}

void handle_button_release_split(GameContext *game_ctx, EventQueue *event_queue){
    game_split(game_ctx);
    game_context_set_game_state(game_ctx, GAME_STATE_PLAYING_SPLIT);
    queue_game_state_event(game_ctx, event_queue);
    event_enqueue(event_queue, (Event){.state={
        .type=STATE_EVENT_SPLIT,
        .data={
            .split={
                .player_cards_in_hand = game_ctx->player->cards_in_hand,
                .player_hand_value = game_ctx->player->hand_value,
                .player_cards_in_split_hand = game_ctx->player->cards_in_split_hand,
                .player_split_hand_value = game_ctx->player->split_hand_value,
                .money = game_ctx->player->money,
                .bet = game_ctx->player->bet,
                .split_bet = game_ctx->player->split_bet
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
    anim_add(anim_pool, (Animation){
        &(game_ctx->cheque_ring_buffer->arr[cheque_idx].rect),
        ANIMATION_TYPE_VEC2,
        ANIMATION_STATE_WAITING,
        animation_cheque_move,
        {.vec2_anim={
            (vec2){game_ctx->cheque_ring_buffer->arr[cheque_idx].rect.pos.x, game_ctx->cheque_ring_buffer->arr[cheque_idx].rect.pos.y},
            (vec2){hmget(game_ctx->cheque_data_map, val).cheque_button_x, hmget(game_ctx->cheque_data_map, val).cheque_button_y}
        }}
    });
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
        anim_add(anim_pool, (Animation){
        &(game_ctx->cheque_ring_buffer->arr[cheque_idx].rect),
        ANIMATION_TYPE_VEC2,
        ANIMATION_STATE_WAITING,
        animation_cheque_move,
        {.vec2_anim={
            (vec2){game_ctx->cheque_ring_buffer->arr[cheque_idx].rect.pos.x, game_ctx->cheque_ring_buffer->arr[cheque_idx].rect.pos.y},
            (vec2){STACK_BUTTON_ORIGIN_X, STACK_BUTTON_ORIGIN_Y}
        }}
    });
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

void handle_animation_card_draw_completed(GameContext *game_ctx, Rect *target){
    if (
        bust(game_ctx->player->hand_value) 
        || blackjack(game_ctx->player->cards_in_hand, game_ctx->player->hand_value)
    ) {
        dealer_reveal_second_card(game_ctx->dealer);
    }
    flip_card(
        (Card *)target, 
        is_second_dealer_card(game_ctx->dealer, (Card *)target),
        dealer_is_hiding_second_card(game_ctx->dealer)
    );
}

void handle_animation_cheque_completed(GameContext *game_ctx, EventQueue *event_queue){
    Cheque cheque = cheque_ring_buffer_dequeue(game_ctx->cheque_ring_buffer);
    if (!cheque.popped){
        player_bet_push(game_ctx->player, cheque.val);
        event_enqueue(event_queue, (Event){
            .state={
                .type=STATE_EVENT_CHEQUE_PUSH_RECEIVED, 
                .data={
                    .cheque_push_received={
                        game_ctx->player->bet,
                        hmget(game_ctx->cheque_data_map, cheque.val).cheque_button_tid
                    }
                }
            }
        });
    }
    else{
        game_ctx->player->money += cheque.val;
        event_enqueue(event_queue, (Event){
            .state={
                .type=STATE_EVENT_CHEQUE_POP_RECEIVED, 
                .data={
                    .cheque_pop_received=
                    {
                        game_ctx->player->money
                    }
                }
            }
        });
    }
}

void update(AppState *as){
    update_delta_time(&(as->prev_frametime), &(as->delta_time));
    while (!event_queue_empty(as->event_queue)){
        Event event = event_dequeue(as->event_queue);
        void *dependencies = NULL;
        switch (event.type){
            case INPUT_EVENT_BUTTON_RELEASE_DEAL:
                handle_button_release_deal(as->game_ctx, as->anim_queue, as->event_queue);
                break;
            case INPUT_EVENT_BUTTON_RELEASE_HIT:
                handle_button_release_hit(as->game_ctx, as->anim_queue, as->event_queue);
                break;
            case INPUT_EVENT_BUTTON_RELEASE_STAND:
                handle_button_release_stand(as->game_ctx, as->anim_queue, as->event_queue);
                break;
            case INPUT_EVENT_BUTTON_RELEASE_BET:
                handle_button_release_bet(as->game_ctx, as->event_queue);
                break;
            case INPUT_EVENT_BUTTON_RELEASE_SPLIT:
                handle_button_release_split(as->game_ctx, as->event_queue);
                break;
            case INPUT_EVENT_BUTTON_RELEASE_STACK: {
                handle_button_release_stack(as->game_ctx, as->anim_pool, as->event_queue);
                break;
            }
            case INPUT_EVENT_BUTTON_RELEASE_WHITE: {
                handle_button_release_cheque(as->game_ctx, as->anim_pool, as->event_queue, CHEQUE_VALUE_ONE);
                break;
            }
            case INPUT_EVENT_BUTTON_RELEASE_RED: {
                handle_button_release_cheque(as->game_ctx, as->anim_pool, as->event_queue, CHEQUE_VALUE_FIVE);
                break;
            }
            case INPUT_EVENT_BUTTON_RELEASE_BLUE: {
                handle_button_release_cheque(as->game_ctx, as->anim_pool, as->event_queue, CHEQUE_VALUE_TEN);
                break;
            }
            case INPUT_EVENT_BUTTON_RELEASE_GREEN: {
                handle_button_release_cheque(as->game_ctx, as->anim_pool, as->event_queue, CHEQUE_VALUE_TWENTY_FIVE);
                break;
            }
            case INPUT_EVENT_BUTTON_RELEASE_BLACK: {
                handle_button_release_cheque(as->game_ctx, as->anim_pool, as->event_queue, CHEQUE_VALUE_HUNDRED);
                break;
            }
            case INPUT_EVENT_BUTTON_RELEASE_PURPLE: {
                handle_button_release_cheque(as->game_ctx, as->anim_pool, as->event_queue, CHEQUE_VALUE_FIVE_HUNDRED);
                break;
            }
            case INPUT_EVENT_BUTTON_RELEASE_YELLOW: {
                handle_button_release_cheque(as->game_ctx, as->anim_pool, as->event_queue, CHEQUE_VALUE_ONE_K);
                break;
            }
            case INPUT_EVENT_BUTTON_RELEASE_ORANGE: {
                handle_button_release_cheque(as->game_ctx, as->anim_pool, as->event_queue, CHEQUE_VALUE_FIVE_K);
                break;
            }
            case INPUT_EVENT_BUTTON_RELEASE_REDBLUE: {
                handle_button_release_cheque(as->game_ctx, as->anim_pool, as->event_queue, CHEQUE_VALUE_TWENTY_FIVE_K);
                break;
            }
            case INPUT_EVENT_BUTTON_RELEASE_GOLD: {
                handle_button_release_cheque(as->game_ctx, as->anim_pool, as->event_queue, CHEQUE_VALUE_HUNDRED_K);
                break;
            }
            case STATE_EVENT_DEAL: 
            case STATE_EVENT_HIT: 
            case STATE_EVENT_STAND:
            case STATE_EVENT_BET:
            case STATE_EVENT_SPLIT:
            case STATE_EVENT_CHEQUE_PUSH_SENT: 
            case STATE_EVENT_CHEQUE_POP_SENT:
            case STATE_EVENT_BET_PAYOUT:
            case STATE_EVENT_SPLIT_HIT: {
                dependencies = (void *)as->font_map;
                break;
            }
            default: {
                break;
            }
        }
        event_listeners_notify_all(&as->event_listeners, event, dependencies);
    }
    animate(as->anim_queue, as->anim_pool, as->event_queue, as->delta_time);
    while (!event_queue_empty(as->event_queue)){
        Event event = event_dequeue(as->event_queue);
        void *dependencies = NULL;
        switch(event.type){
            case ANIMATION_EVENT_ANIMATION_CARD_DRAW_COMPLETED:
                handle_animation_card_draw_completed(as->game_ctx, event.anim.target);
                break;
            case ANIMATION_EVENT_ANIMATION_CHEQUE_COMPLETED: {
                handle_animation_cheque_completed(as->game_ctx, as->event_queue);
                break;
            }
            case STATE_EVENT_CHEQUE_PUSH_RECEIVED:
            case STATE_EVENT_CHEQUE_POP_RECEIVED: {
                dependencies = (void *)as->font_map;
                break;
            }
            default: {
                break;
            }
        }
        event_listeners_notify_all(&as->event_listeners, event, dependencies);
    }
}