#pragma once

#include "game_types.h"

GameContext* game_context_create(void);
void game_context_destroy(GameContext *game_ctx);
GAME_STATE game_context_get_game_state(GameContext *game_ctx);
GAME_STATE game_context_get_prev_game_state(GameContext *game_ctx);
void game_context_set_game_state(GameContext *game_ctx, GAME_STATE state);
void game_reset(GameContext *game_ctx);
void game_split(GameContext *game_ctx);

void game_cheque_ring_buffer_enqueue(ChequeRingBuffer *cheque_ring_buffer, Cheque cheque);
Cheque game_cheque_ring_buffer_dequeue(ChequeRingBuffer *cheque_ring_buffer);

void game_deck_shuffle(Card *deck, int *p_deck_top_index);

bool bust(int hand_value);
bool blackjack(int cards_in_hand, int hand_value);
bool can_hit(int cards_in_hand, int hand_value);
void game_flip_card(Card *card, bool is_second_dealer_card, bool dealer_is_hiding_second_card);

bool game_dealer_is_hiding_second_card(Dealer *dealer);
void game_dealer_reveal_second_card(Dealer *dealer);
bool game_card_is_second_dealer_card(Dealer *dealer, Card *card);
Card* game_dealer_hit(Card* deck, int *deck_top_index_ptr, Dealer *dealer);
Card** game_dealer_get_hand(Dealer *dealer);
bool game_can_insure(Dealer *dealer);

Card* game_player_hit(Card *deck, int *deck_top_index_ptr, Player *player);
Card* game_player_hit_split(Card *deck, int *deck_top_index_ptr, Player *player);
void game_player_bet_push(Player *player, CHEQUE_VALUE val);
CHEQUE_VALUE game_player_bet_pop(Player *player);
CHEQUE_VALUE game_player_bet_peek(Player *player);
bool game_player_can_bet(Player *player);
bool game_player_can_double_down(Player *player);