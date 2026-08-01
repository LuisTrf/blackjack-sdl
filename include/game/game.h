#pragma once

#include <stdbool.h>
#include "game_constants.h"
#include "../rect.h"
#include "game.h"
#include "../vec2.h"

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

typedef enum CHIP_VALUE {
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
    Rect rect;
    CARD_LOCATION location;
    char suit;
    char rank;
    int rank_value;
    bool face_down;
} Card;

typedef struct Deck {
    int top;
    Card *arr;
} Deck;

typedef struct Player {
    Card *hand[PLAYER_MAXIMUM_HAND_SIZE];
    unsigned char hand_value;
    unsigned char cards_in_hand;
    unsigned char aces_in_hand_worth_11;
    float money;
    float bet;
    int betted_chips;
    /* replace array via stb dynamic arr */
    CHIP_VALUE bet_history[MAXIMUM_BETTED_CHIPS];
} Player;

typedef struct Dealer{
    Card *hand[DEALER_MAXIMUM_HAND_SIZE];
    unsigned char hand_value;
    unsigned char cards_in_hand;
    unsigned char aces_in_hand_worth_11;
} Dealer;

typedef struct GameContext {
    GAME_STATE game_state;
    GAME_STATE prev_game_state;
    Deck *deck;
    Player *player;
    Dealer *dealer;
} GameContext;

GameContext* game_context_create(void);
void game_context_destroy(GameContext *game_ctx);
GAME_STATE game_context_get_game_state(GameContext *game_ctx);
GAME_STATE game_context_get_prev_game_state(GameContext *game_ctx);
void game_context_set_game_state(GameContext *game_ctx, GAME_STATE state);
void game_reset(GameContext *game_ctx);

void deck_shuffle(Deck *deck);
int deck_get_card_count(Deck *deck);

bool dealer_bust(Dealer *dealer);
bool dealer_is_blackjack(Dealer *dealer);
bool dealer_can_hit(Dealer *dealer);
bool dealer_is_hiding_second_card(Dealer *dealer);
void dealer_reveal_second_card(Dealer *dealer);
Card* dealer_hit(Deck* deck, Dealer *dealer);
Card** dealer_get_hand(Dealer *dealer);
bool can_insure(Dealer *dealer);

bool player_bust(Player *player);
bool player_is_blackjack(Player *player);
bool player_can_hit(Player *player);
Card* player_hit(Deck *deck, Player *player);
bool player_is_bet_history_empty(Player *player);
bool player_is_bet_history_full(Player *player);
bool player_bet_push(Player *player, CHIP_VALUE val);
CHIP_VALUE player_bet_pop(Player *player);
CHIP_VALUE player_bet_peek(Player *player);
bool player_can_bet(Player *player);
bool player_can_double_down(Player *player);