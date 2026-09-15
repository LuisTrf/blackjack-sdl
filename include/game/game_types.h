#pragma once

#include "game_constants.h"
#include "game_enums.h"
#include "../rect.h"
#include "../render/render_types.h"

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
    Card* split_hand[MAXIMUM_HAND_SIZE];
    int split_hand_value;
    int cards_in_split_hand;
    int aces_in_split_hand_worth_11;
    float money;
    float bet;
    float split_bet;
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