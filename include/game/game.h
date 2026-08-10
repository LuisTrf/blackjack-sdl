#pragma once

#include <stdbool.h>
#include "game_constants.h"
#include "../rect.h"
#include "../vec2.h"
#include "../render/render_types.h"

typedef enum CARD_LOCATION {
    CARD_LOCATION_DECK,
    CARD_LOCATION_PLAYER_HAND,
    CARD_LOCATION_DEALER_HAND
} CARD_LOCATION;

typedef enum GAME_STATE {
    _GAME_STATE_NONE,
    GAME_STATE_NEW,
    GAME_STATE_BETTING,
    GAME_STATE_PLAYING
} GAME_STATE;

typedef enum CHEQUE_VALUE {
    _CHEQUE_VALUE_NONE = 0,
    CHEQUE_VALUE_ONE = 1,
    CHEQUE_VALUE_FIVE = 5,
    CHEQUE_VALUE_TEN = 10,
    CHEQUE_VALUE_TWENTY_FIVE = 25,
    CHEQUE_VALUE_HUNDRED = 100,
    CHEQUE_VALUE_FIVE_HUNDRED = 500,
    CHEQUE_VALUE_ONE_K = 1000,
    CHEQUE_VALUE_FIVE_K = 5000,
    CHEQUE_VALUE_TWENTY_FIVE_K = 25000,
    CHEQUE_VALUE_HUNDRED_K = 100000
} CHEQUE_VALUE;

typedef struct Cheque {
    Rect rect;
    CHEQUE_VALUE val;
    TEXTURE_ID tid;
    bool popped;
} Cheque;

typedef struct ChequeRingBuffer {
    int size;
    int count;
    int head;
    int tail;
    Cheque *arr;
} ChequeRingBuffer;

typedef struct cheque_data_t {
    TEXTURE_ID cheque_tid;
    TEXTURE_ID cheque_button_tid;
    float cheque_button_x;
    float cheque_button_y;
} cheque_data;

typedef struct cheque_data_hash_t {
    CHEQUE_VALUE key;
    cheque_data value;
} cheque_data_hash;

typedef struct Card {
    Rect rect;
    CARD_LOCATION location;
    char suit;
    char rank;
    int rank_value;
    bool face_down;
} Card;

typedef struct Player {
    Card* hand[MAXIMUM_HAND_SIZE];
    int hand_value;
    int cards_in_hand;
    int aces_in_hand_worth_11;
    float money;
    float bet;
    int bet_count;
    CHEQUE_VALUE *bet_stack;
} Player;

typedef struct Dealer{
    Card* hand[MAXIMUM_HAND_SIZE];
    int hand_value;
    int cards_in_hand;
    int aces_in_hand_worth_11;
} Dealer;

typedef struct GameContext {
    GAME_STATE game_state;
    GAME_STATE prev_game_state;
    ChequeRingBuffer *cheque_ring_buffer;
    cheque_data_hash* cheque_data_map;
    Card* deck;
    int *deck_top_index_ptr;
    Dealer *dealer;
    Player *player;
} GameContext;

GameContext* game_context_create(void);
void game_context_destroy(GameContext *game_ctx);
GAME_STATE game_context_get_game_state(GameContext *game_ctx);
GAME_STATE game_context_get_prev_game_state(GameContext *game_ctx);
void game_context_set_game_state(GameContext *game_ctx, GAME_STATE state);
void game_reset(GameContext *game_ctx);

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
void player_bet_push(Player *player, CHEQUE_VALUE val);
CHEQUE_VALUE player_bet_pop(Player *player);
CHEQUE_VALUE player_bet_peek(Player *player);
bool player_can_bet(Player *player);
bool player_can_double_down(Player *player);