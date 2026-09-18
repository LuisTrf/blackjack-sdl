#include "../../vendored/stb/stb_ds.h"
#include "../../include/event/event.h"
#include "../../include/game/game.h"
#include "../../include/game/card_constants.h"
#include "../../include/update/animation.h"
#include "../../include/event/event_handler.h"

void _event_handler_queue_game_state_event(GameContext *game_ctx, EventQueue *event_queue){
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

void _event_handler_queue_bet_payout_event(EventQueue *event_queue, float money){
    event_enqueue(event_queue, (Event){
        .state={
            .type=STATE_EVENT_BET_PAYOUT,
            .data={
                .bet_payout = {money}
            }
        }
    });
}

void _event_handler_queue_card_animation(AnimationQueue *anim_queue, Card *tgt_card, vec2 dst){
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

void _event_handler_queue_deal_state_event(GameContext *game_ctx, EventQueue *event_queue){
    event_enqueue(event_queue, (Event){.state={
        .type=STATE_EVENT_DEAL,
        .data={
            .deal={
                .game_state = game_context_get_game_state(game_ctx),
                .dealer_cards_in_hand=game_ctx->dealer->cards_in_hand,
                .dealer_hand_value=game_ctx->dealer->hand_value,
                .player_cards_in_hand=game_ctx->player->cards_in_hand,
                .player_hand_value=game_ctx->player->hand_value,
                .dealer_first_card_rank=game_ctx->dealer->hand[0]->rank
            }
        }
    }});
}

void event_handler_handle_deal_input_event(AppState *as){
    GameContext *game_ctx = as->game_ctx;
    EventQueue *event_queue = as->event_queue;
    AnimationQueue *anim_queue = as->anim_queue;

    if (game_context_get_game_state(game_ctx) == GAME_STATE_BETTING) {
        game_context_set_game_state(game_ctx, GAME_STATE_BETTING_PLAYING);
    }
    else{
        game_reset(game_ctx);
        game_context_set_game_state(game_ctx, GAME_STATE_PLAYING);
    }
    _event_handler_queue_game_state_event(game_ctx, event_queue);

    game_deck_shuffle(game_ctx->deck, &game_ctx->deck_top_index);

    Card* dc1 = game_dealer_hit(game_ctx->deck, &game_ctx->deck_top_index, game_ctx->dealer);
    _event_handler_queue_card_animation(anim_queue, dc1, (vec2){HAND_ORIGIN_X, HAND_ORIGIN_Y_DEALER});
    Card* dc2 = game_dealer_hit(game_ctx->deck, &game_ctx->deck_top_index, game_ctx->dealer);
    _event_handler_queue_card_animation(anim_queue, dc2, (vec2){(HAND_ORIGIN_X + HAND_STEP_X), HAND_ORIGIN_Y_DEALER});

    Card *pc1 = game_player_hit(game_ctx->deck, &game_ctx->deck_top_index, game_ctx->player);
    _event_handler_queue_card_animation(anim_queue, pc1, (vec2){HAND_ORIGIN_X, HAND_ORIGIN_Y_PLAYER});
    Card *pc2 = game_player_hit(game_ctx->deck, &game_ctx->deck_top_index, game_ctx->player);
    _event_handler_queue_card_animation(anim_queue, pc2, (vec2){(HAND_ORIGIN_X + HAND_STEP_X), HAND_ORIGIN_Y_PLAYER});

    if (
        game_context_get_game_state(game_ctx) == GAME_STATE_BETTING_PLAYING
        && blackjack(game_ctx->player->cards_in_hand, game_ctx->player->hand_value)
    ){
        if (!blackjack(game_ctx->dealer->cards_in_hand, game_ctx->dealer->hand_value)){
            game_ctx->player->money += game_ctx->player->bet * PLAYER_BLACKJACK_BET_PAYOUT;
            _event_handler_queue_bet_payout_event(event_queue, game_ctx->player->money);
        }
        else {
            if (game_ctx->dealer->hand[0]->rank != 'A'){
                game_ctx->player->money += game_ctx->player->bet;
                _event_handler_queue_bet_payout_event(event_queue, game_ctx->player->money);
            }
        }
    }

    if (
        pc1->rank_value == pc2->rank_value
        && (game_context_get_game_state(game_ctx) == GAME_STATE_BETTING_PLAYING)
        && game_ctx->player->money >= game_ctx->player->bet
    ){
        event_enqueue(event_queue, (Event){.state={.type=STATE_EVENT_SPLIT_POSSIBLE}});
    }

    if (
        game_context_get_game_state(game_ctx) == GAME_STATE_BETTING_PLAYING
        && game_ctx->player->money >= game_ctx->player->bet
    )
    {
        event_enqueue(event_queue, (Event){.state={.type=STATE_EVENT_DOUBLE_DOWN_POSSIBLE}});
    }

    if (
        game_context_get_game_state(game_ctx) == GAME_STATE_BETTING_PLAYING
        && game_ctx->dealer->hand[0]->rank == 'A'
        && game_ctx->player->money >= game_ctx->player->bet/2.f
    ){
        event_enqueue(event_queue, (Event){.state={.type=STATE_EVENT_INSURANCE_POSSIBLE}});
    }

    _event_handler_queue_deal_state_event(game_ctx, event_queue);
}

void _event_handler_queue_bet_input_event(GameContext *game_ctx, EventQueue *event_queue){
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

void event_handler_handle_bet_input_event(AppState *as){
    game_reset(as->game_ctx);
    game_context_set_game_state(as->game_ctx, GAME_STATE_BETTING);
    _event_handler_queue_bet_input_event(as->game_ctx, as->event_queue);
}

float _event_handler_card_y_get_from_game_state(GameContext *game_ctx){
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
    return card_y;
}

float _event_handler_card_x_get_from_game_state(GameContext *game_ctx){
    GAME_STATE game_state = game_context_get_game_state(game_ctx);
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
    return card_x;
}

void _event_handler_queue_split_hit_state_event(GameContext *game_ctx, EventQueue *event_queue){
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
}

void _event_handler_queue_hit_state_event(GameContext *game_ctx, EventQueue *event_queue){
    event_enqueue(event_queue, (Event){.state={
        .type=STATE_EVENT_HIT,
        .data={
            .hit={
                .dealer_cards_in_hand=game_ctx->dealer->cards_in_hand,
                .dealer_hand_value=game_ctx->dealer->hand_value,
                .player_cards_in_hand=game_ctx->player->cards_in_hand,
                .player_hand_value=game_ctx->player->hand_value,
                .player_cards_in_split_hand=game_ctx->player->cards_in_split_hand,
                .player_split_hand_value=game_ctx->player->split_hand_value,
                .money=game_ctx->player->money
            }
        }
    }});
}

void _event_handler_payout_split_hand(GameContext *game_ctx, EventQueue *event_queue){
    if (
        !bust(game_ctx->player->split_hand_value)
        && !blackjack(game_ctx->dealer->cards_in_hand, game_ctx->dealer->hand_value)
    ){
        if (
            game_ctx->player->split_hand_value > game_ctx->dealer->hand_value
            || bust(game_ctx->dealer->hand_value)
        ){
            game_ctx->player->money += game_ctx->player->split_bet * STANDARD_BET_PAYOUT;
            _event_handler_queue_bet_payout_event(event_queue, game_ctx->player->money);
        }
        else if (
            game_ctx->player->hand_value == game_ctx->dealer->hand_value
            && !blackjack(game_ctx->dealer->cards_in_hand, game_ctx->dealer->hand_value)
        ){
            game_ctx->player->money += game_ctx->player->split_bet;
            _event_handler_queue_bet_payout_event(event_queue, game_ctx->player->money);
        }
    }
}

void _event_handler_payout_hand(GameContext *game_ctx, EventQueue *event_queue){
    if (
        !bust(game_ctx->player->hand_value)
        && !blackjack(game_ctx->dealer->cards_in_hand, game_ctx->dealer->hand_value)
    ){
        if (
            game_ctx->player->hand_value > game_ctx->dealer->hand_value 
            || bust(game_ctx->dealer->hand_value)
        ){
            game_ctx->player->money += game_ctx->player->bet * STANDARD_BET_PAYOUT;
            _event_handler_queue_bet_payout_event(event_queue, game_ctx->player->money);
        }
        else if (
            game_ctx->player->hand_value == game_ctx->dealer->hand_value 
            && !blackjack(game_ctx->dealer->cards_in_hand, game_ctx->dealer->hand_value)
        ){
            game_ctx->player->money += game_ctx->player->bet;
            _event_handler_queue_bet_payout_event(event_queue, game_ctx->player->money);
        }
    }
}

void event_handler_handle_hit_input_event(AppState *as){
    GameContext *game_ctx = as->game_ctx;
    EventQueue *event_queue = as->event_queue;
    AnimationQueue *anim_queue = as->anim_queue;

    GAME_STATE game_state = game_context_get_game_state(game_ctx);
    float card_y = _event_handler_card_y_get_from_game_state(game_ctx);
    Card *pc = (game_state == GAME_STATE_PLAYING_SPLIT) ? 
        game_player_hit_split(game_ctx->deck, &game_ctx->deck_top_index, game_ctx->player) :
        game_player_hit(game_ctx->deck, &game_ctx->deck_top_index, game_ctx->player);
    float card_x = _event_handler_card_x_get_from_game_state(game_ctx);
    _event_handler_queue_card_animation(anim_queue, pc, (vec2){card_x, card_y});

    if (game_state == GAME_STATE_PLAYING_SPLIT){
        _event_handler_queue_split_hit_state_event(game_ctx, event_queue);
        if (bust(game_ctx->player->split_hand_value)){
            game_context_set_game_state(game_ctx, GAME_STATE_BETTING_PLAYING);
            _event_handler_queue_game_state_event(game_ctx, event_queue);
        }
    }
    else {
        _event_handler_queue_hit_state_event(game_ctx, event_queue);
        if (bust(game_ctx->player->hand_value)){
            if (game_context_get_prev_game_state(game_ctx) == GAME_STATE_PLAYING_SPLIT){
                _event_handler_payout_split_hand(game_ctx, event_queue);
            }
            game_context_set_game_state(game_ctx, GAME_STATE_FIN);
            _event_handler_queue_game_state_event(game_ctx, event_queue);
        }
    }
}

void _event_handler_queue_stand_state_event(GameContext *game_ctx, EventQueue *event_queue){
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
                    .player_split_hand_value=game_ctx->player->split_hand_value,
                    .money=game_ctx->player->money
                }
            }
        }
    });
}

void event_handler_handle_stand_input_event(AppState *as){
    GameContext *game_ctx = as->game_ctx;
    EventQueue *event_queue = as->event_queue;
    AnimationQueue *anim_queue = as->anim_queue;
    track_hash* track_map = as->track_map;

    if (game_context_get_game_state(game_ctx) == GAME_STATE_PLAYING_SPLIT){
        game_context_set_game_state(game_ctx, GAME_STATE_BETTING_PLAYING);
        _event_handler_queue_game_state_event(game_ctx, event_queue);
        if (game_ctx->player->money >= game_ctx->player->bet){
            event_enqueue(event_queue, (Event){.state={.type=STATE_EVENT_DOUBLE_DOWN_POSSIBLE}});
        }
        event_enqueue(event_queue, (Event){.state={.type=STATE_EVENT_SPLIT_STAND}});
        return;
    }

    game_dealer_reveal_second_card(game_ctx->dealer);
    if (game_ctx->dealer->hand_value > 16){
        MIX_PlayTrack(hmget(track_map, TRACK_ID_CARD_FLIP), 0);
    }

    float dealer_hand_y = (game_context_get_prev_game_state(game_ctx) == GAME_STATE_PLAYING_SPLIT) ? 
        HAND_SPLITTING_Y_DEALER :
        HAND_ORIGIN_Y_DEALER;
    while (game_ctx->dealer->hand_value < 17){
        Card *dc = game_dealer_hit(game_ctx->deck, &game_ctx->deck_top_index, game_ctx->dealer);
        _event_handler_queue_card_animation(anim_queue, dc, (vec2){(HAND_ORIGIN_X + HAND_STEP_X*(game_ctx->dealer->cards_in_hand-1)), dealer_hand_y});
    }

    if (game_context_get_game_state(game_ctx) == GAME_STATE_BETTING_PLAYING){
        _event_handler_payout_hand(game_ctx, event_queue);
    }
    if (game_context_get_prev_game_state(game_ctx) == GAME_STATE_PLAYING_SPLIT){
        _event_handler_payout_split_hand(game_ctx, event_queue);
    }

    if (blackjack(game_ctx->dealer->cards_in_hand, game_ctx->dealer->hand_value)){
        MIX_PlayTrack(hmget(track_map, TRACK_ID_BLACKJACK_LOSS), 0);
    }

    _event_handler_queue_stand_state_event(game_ctx, event_queue);

    game_context_set_game_state(game_ctx, GAME_STATE_FIN);
    _event_handler_queue_game_state_event(game_ctx, event_queue);
}

void _event_handler_queue_split_state_event(GameContext *game_ctx, EventQueue *event_queue){
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

void event_handler_handle_split_input_event(AppState *as){
    GameContext *game_ctx = as->game_ctx;
    EventQueue *event_queue = as->event_queue;

    game_split(game_ctx);

    game_context_set_game_state(game_ctx, GAME_STATE_PLAYING_SPLIT);
    _event_handler_queue_game_state_event(game_ctx, event_queue);

    _event_handler_queue_split_state_event(game_ctx, event_queue);
}

void _event_handler_queue_double_down_state_event(GameContext *game_ctx, EventQueue *event_queue){
    event_enqueue(event_queue, (Event){.state={
        .type=STATE_EVENT_DOUBLE_DOWN,
        .data={
            .double_down={
                .game_state = game_context_get_game_state(game_ctx),
                .prev_game_state = game_context_get_prev_game_state(game_ctx),
                .money = game_ctx->player->money,
                .bet = game_ctx->player->bet,
                .split_bet = game_ctx->player->split_bet
            }
        }
    }});
}

void event_handler_handle_double_down_input_event(AppState *as){
    GameContext *game_ctx = as->game_ctx;
    EventQueue *event_queue = as->event_queue;

    GAME_STATE game_state = game_context_get_game_state(game_ctx);
    if (game_state == GAME_STATE_BETTING_PLAYING){
        game_ctx->player->money -= game_ctx->player->bet;
        game_ctx->player->bet *= 2;
    }
    else if (game_state == GAME_STATE_PLAYING_SPLIT){
        game_ctx->player->money -= game_ctx->player->split_bet;
        game_ctx->player->split_bet *= 2;
    }

    event_handler_handle_hit_input_event(as);

    _event_handler_queue_double_down_state_event(game_ctx, event_queue);
}

void _event_handler_queue_insurance_state_event(GameContext *game_ctx, EventQueue *event_queue){
    event_enqueue(event_queue, (Event){.state={
        .type=STATE_EVENT_INSURANCE,
        .data={
            .insurance={
                .dealer_cards_in_hand = game_ctx->dealer->cards_in_hand,
                .dealer_hand_value = game_ctx->dealer->hand_value,
                .player_cards_in_hand = game_ctx->player->cards_in_hand,
                .player_hand_value = game_ctx->player->hand_value,
                .money = game_ctx->player->money,
                .bet = game_ctx->player->bet,
            }
        }
    }});
}

void event_handler_handle_insure_input_event(AppState *as){
    GameContext *game_ctx = as->game_ctx;
    EventQueue *event_queue = as->event_queue;

    float side_bet = game_ctx->player->bet/2.f;
    game_ctx->player->money -= side_bet;
    if (blackjack(game_ctx->dealer->cards_in_hand, game_ctx->dealer->hand_value)){
        game_ctx->player->money += side_bet * INSURANCE_BET_PAYOUT;
    }

    event_handler_handle_stand_input_event(as);

    _event_handler_queue_insurance_state_event(game_ctx, event_queue);
    
    game_context_set_game_state(game_ctx, GAME_STATE_FIN);
    _event_handler_queue_game_state_event(game_ctx, event_queue);
}

void _event_handler_cheque_ring_buffer_enqueue_new_sourced_on_stack(GameContext *game_ctx, cheque_data_hash* cheque_data_map, CHEQUE_VALUE val){
    game_cheque_ring_buffer_enqueue(
        game_ctx->cheque_ring_buffer, 
        (Cheque){
            {
                {
                    STACK_BUTTON_ORIGIN_X, 
                    STACK_BUTTON_ORIGIN_Y
                },
                CHIP_BUTTON_WIDTH,
                CHIP_BUTTON_HEIGHT,
                CHIP_BUTTON_WIDTH,
                CHIP_BUTTON_HEIGHT,
                true
            },
            val,
            hmget(cheque_data_map, val).cheque_tid,
            false
        }
    );
}

void _event_handler_queue_cheque_popped_animation(GameContext *game_ctx, cheque_data_hash* cheque_data_map, AnimationPool *anim_pool, int cheque_idx, CHEQUE_VALUE val){
    float* p_dst_x = hmget(cheque_data_map, val).p_cheque_button_x;
    float* p_dst_y = hmget(cheque_data_map, val).p_cheque_button_y;
    anim_add(anim_pool, (Animation){
        &(game_ctx->cheque_ring_buffer->arr[cheque_idx].rect),
        ANIMATION_TYPE_VEC2,
        ANIMATION_STATE_WAITING,
        animation_cheque_move,
        {.vec2_anim={
            (vec2){game_ctx->cheque_ring_buffer->arr[cheque_idx].rect.pos.x, game_ctx->cheque_ring_buffer->arr[cheque_idx].rect.pos.y},
            (vec2){*p_dst_x, *p_dst_y}
        }}
    });
}

void _event_handler_queue_cheque_pop_sent_state_event(GameContext *game_ctx, TEXTURE_ID stack_tid, EventQueue *event_queue){
    event_enqueue(
        event_queue, 
        (Event){
            .state={
                .type=STATE_EVENT_CHEQUE_POP_SENT, 
                .data={
                    .cheque_pop_sent={
                        stack_tid,
                        game_ctx->player->bet, 
                    }
                }
            }
        }
    );
}

void event_handler_handle_stack_input_event(AppState *as){
    GameContext *game_ctx = as->game_ctx;
    EventQueue *event_queue = as->event_queue;
    AnimationPool *anim_pool = as->anim_pool;

    int cheque_idx = game_ctx->cheque_ring_buffer->tail;
    CHEQUE_VALUE val = game_player_bet_pop(game_ctx->player);
    CHEQUE_VALUE val_underneath = game_player_bet_peek(game_ctx->player);
    TEXTURE_ID stack_tid = (val_underneath == _CHEQUE_VALUE_NONE) ? TEXTURE_ID_NULL : hmget(as->cheque_data_map, val_underneath).cheque_button_tid;
    
    _event_handler_cheque_ring_buffer_enqueue_new_sourced_on_stack(game_ctx, as->cheque_data_map, val);
    
    _event_handler_queue_cheque_popped_animation(game_ctx, as->cheque_data_map, anim_pool, cheque_idx, val);

    _event_handler_queue_cheque_pop_sent_state_event(game_ctx, stack_tid, event_queue);
}

void _event_handler_cheque_ring_buffer_enqueue_new_sourced_on_cheque(GameContext *game_ctx, cheque_data_hash* cheque_data_map, CHEQUE_VALUE val){
    float* p_src_x = hmget(cheque_data_map, val).p_cheque_button_x;
    float* p_src_y = hmget(cheque_data_map, val).p_cheque_button_y;
    game_cheque_ring_buffer_enqueue(
        game_ctx->cheque_ring_buffer,
        (Cheque){
            {
                {
                    *p_src_x,
                    *p_src_y
                },
                CHIP_BUTTON_WIDTH,
                CHIP_BUTTON_HEIGHT,
                CHIP_BUTTON_WIDTH,
                CHIP_BUTTON_HEIGHT,
                true
            },
            val,
            hmget(cheque_data_map, val).cheque_tid,
            true
        }
    );
}

void _event_handler_queue_cheque_pushed_animation(GameContext *game_ctx, AnimationPool *anim_pool, int cheque_idx, CHEQUE_VALUE val){
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
}

void _event_handler_queue_cheque_push_sent_state_event(GameContext *game_ctx, EventQueue *event_queue){
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

void event_handler_handle_cheque_input_event(AppState *as, CHEQUE_VALUE val){
    GameContext *game_ctx = as->game_ctx;
    EventQueue *event_queue = as->event_queue;
    AnimationPool *anim_pool = as->anim_pool;

    int cheque_idx = game_ctx->cheque_ring_buffer->tail;
    game_ctx->player->money -= val;
    
    _event_handler_cheque_ring_buffer_enqueue_new_sourced_on_cheque(game_ctx, as->cheque_data_map, val);

    _event_handler_queue_cheque_pushed_animation(game_ctx, anim_pool, cheque_idx, val);

    _event_handler_queue_cheque_push_sent_state_event(game_ctx, event_queue);
}

void event_handler_handle_card_draw_completed_animation_event(AppState *as, Rect *target){
    GameContext *game_ctx = as->game_ctx;
    AnimationQueue *anim_queue = as->anim_queue;
    track_hash* track_map = as->track_map;

    if (
        (
            bust(game_ctx->player->hand_value) 
            || blackjack(game_ctx->player->cards_in_hand, game_ctx->player->hand_value)
        )
        && anim_queue_empty(anim_queue)
    ) {
        game_dealer_reveal_second_card(game_ctx->dealer);
    }

    if (
        blackjack(game_ctx->player->cards_in_hand, game_ctx->player->hand_value)
        && anim_queue_empty(anim_queue)
    ){
        MIX_PlayTrack(hmget(track_map, TRACK_ID_BLACKJACK_WIN), 0);
    }
    
    game_flip_card(
        (Card *)target, 
        game_card_is_second_dealer_card(game_ctx->dealer, (Card *)target),
        game_dealer_is_hiding_second_card(game_ctx->dealer)
    );
}

void _event_handler_queue_cheque_push_received_state_event(GameContext *game_ctx, cheque_data_hash* cheque_data_map, EventQueue *event_queue, CHEQUE_VALUE val){
    event_enqueue(event_queue, (Event){
        .state={
            .type=STATE_EVENT_CHEQUE_PUSH_RECEIVED, 
            .data={
                .cheque_push_received={
                    hmget(cheque_data_map, val).cheque_button_tid,
                    game_ctx->player->bet,
                }
            }
        }
    });
}

void _event_handler_queue_cheque_pop_received_state_event(GameContext *game_ctx, EventQueue *event_queue){
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

void event_handler_handle_cheque_completed_animation_event(AppState *as){
    GameContext *game_ctx = as->game_ctx;
    EventQueue *event_queue = as->event_queue;

    Cheque cheque = game_cheque_ring_buffer_dequeue(game_ctx->cheque_ring_buffer);
    if (cheque.pushed){
        game_player_bet_push(game_ctx->player, cheque.val);
        _event_handler_queue_cheque_push_received_state_event(game_ctx, as->cheque_data_map, event_queue, cheque.val);
    }
    else{
        game_ctx->player->money += cheque.val;
        _event_handler_queue_cheque_pop_received_state_event(game_ctx, event_queue);
    }
}
