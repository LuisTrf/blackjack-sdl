#pragma once

#include "game_types.h"

GameContext* game_context_create(void);
void game_context_destroy(GameContext *game_ctx);
GAME_STATE game_context_get_game_state(GameContext *game_ctx);
GAME_STATE game_context_get_prev_game_state(GameContext *game_ctx);
void game_context_set_game_state(GameContext *game_ctx, GAME_STATE state);
void game_reset(GameContext *game_ctx);
void game_split(GameContext *game_ctx);

void cheque_ring_buffer_enqueue(ChequeRingBuffer *cheque_ring_buffer, Cheque cheque);
Cheque cheque_ring_buffer_dequeue(ChequeRingBuffer *cheque_ring_buffer);

void deck_shuffle(Card *deck, int *deck_top_index_ptr);

bool bust(int hand_value);
bool blackjack(int cards_in_hand, int hand_value);
void flip_card(Card *card, bool is_second_dealer_card, bool dealer_is_hiding_second_card);
bool can_hit(int cards_in_hand, int hand_value);

bool dealer_is_hiding_second_card(Dealer *dealer);
void dealer_reveal_second_card(Dealer *dealer);
bool is_second_dealer_card(Dealer *dealer, Card *card);
Card* dealer_hit(Card* deck, int *deck_top_index_ptr, Dealer *dealer);
Card** dealer_get_hand(Dealer *dealer);
bool can_insure(Dealer *dealer);

Card* player_hit(Card *deck, int *deck_top_index_ptr, Player *player);
Card* player_hit_split(Card *deck, int *deck_top_index_ptr, Player *player);
void player_bet_push(Player *player, CHEQUE_VALUE val);
CHEQUE_VALUE player_bet_pop(Player *player);
CHEQUE_VALUE player_bet_peek(Player *player);
bool player_can_bet(Player *player);
bool player_can_double_down(Player *player);