#pragma once

#include <stdbool.h>

typedef enum {
    GAME_STATE_NEW,
    GAME_STATE_BETTING,
    GAME_STATE_PLAYING
} GAME_STATE;

typedef enum {
    CARD_LOCATION_DECK,
    CARD_LOCATION_PLAYER_HAND,
    CARD_LOCATION_DEALER_HAND
} CARD_LOCATION;

typedef enum {
    _CHIP_VALUE_NONE = 0,
    CHIP_VALUE_ONE = 1,
    CHIP_VALUE_FIVE = 5,
    CHIP_VALUE_TEN = 10,
    CHIP_VALUE_TWENTY_FIVE = 25,
    CHIP_VALUE_HUNDRED = 100,
    CHIP_VALUE_FIVE_HUNDRED = 500,
    CHIP_VALUE_ONE_K = 1000,
    CHIP_VALUE_FIVE_K = 5000,
    CHIP_VALUE_TWENTY_FIVE_K = 25000,
    CHIP_VALUE_HUNDRED_K = 100000
} CHIP_VALUE;

typedef struct Card {
    CARD_LOCATION location;
    char suit;
    char rank;
    int rank_value;
    bool face_down;
} Card;

#define PLAYER_MAXIMUM_HAND_SIZE 5
#define DEALER_MAXIMUM_HAND_SIZE 10
#define MAXIMUM_BETTED_CHIPS 256
#define PLAYER_BEGINNING_MONEY 100000.00
#define STANDARD_BET_PAYOUT 2.f
#define PLAYER_BLACKJACK_BET_PAYOUT 1.5f

extern const char suits[4];
extern const char ranks[13];
extern const int rank_values[13];

GAME_STATE get_game_state(void);
GAME_STATE get_prev_game_state(void);
void set_game_state(GAME_STATE state);

void allocate_deck_memory(void);
void free_deck_memory(void);
void shuffle_deck(void);
void reset_game_state(void);

bool dealer_bust(void);
bool player_bust(void);
bool dealer_blackjack(void);
bool player_blackjack(void);
bool dealer_can_hit(void);
bool player_can_hit(void);
bool is_second_dealer_card_hidden(void);

Card* dealer_hit(void);
Card* player_hit(void);
void reveal_second_dealer_card(void);
Card** get_dealer_hand(void);
Card** get_player_hand(void);
int get_dealer_hand_value(void);
int get_player_hand_value(void);
int get_player_cards_in_hand(void);
int get_dealer_cards_in_hand(void);

double get_player_money(void);
void add_to_player_bet(double money);
void sub_from_player_bet(double money);
void add_to_player_money(double money);
void sub_from_player_money(double money);
double get_player_bet(void);
bool is_bet_history_empty(void);
bool is_bet_history_full(void);
bool push_bet(CHIP_VALUE val);
CHIP_VALUE pop_bet(void);
CHIP_VALUE peek_bet(void);
bool can_player_bet(void);

bool can_double_down(void);
bool can_insure(void);
void insure(void);
bool insured(void);