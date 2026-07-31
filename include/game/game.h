#pragma once

#include <stdbool.h>
#include "game_constants.h"
#include "gameobj.h"
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
    GameObject obj;
    CARD_LOCATION location;
    char suit;
    char rank;
    int rank_value;
    bool face_down;
} Card;

typedef struct Deck {
    int top;
    Card** arr;
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

typedef struct Game_Context {
    GAME_STATE game_state;
    GAME_STATE prev_game_state;
    Deck *deck;
    Player *player;
    Dealer *dealer;
} Game_Context;

Game_Context* game_context_create(void);
void game_context_destroy(Game_Context *p_gc);
GAME_STATE game_context_get_game_state(Game_Context *p_gc);
GAME_STATE game_context_get_prev_game_state(Game_Context *p_gc);
void game_context_set_game_state(Game_Context *p_gc, GAME_STATE state);
void game_reset(Game_Context *p_gc);

void game_shuffle_deck(Deck *deck);
Card* game_draw_random_card(Deck *deck);
int deck_get_card_count(Deck *deck);

bool game_dealer_bust(Dealer *dealer);
bool game_dealer_is_blackjack(Dealer *dealer);
bool game_dealer_can_hit(Dealer *dealer);
bool game_dealer_is_hiding_second_card(Dealer *dealer);
void game_dealer_reveal_second_card(Dealer *dealer);
Card* game_dealer_hit(Deck* deck, Dealer *dealer);
Card** game_dealer_get_hand(Dealer *dealer);
int game_dealer_get_hand_value(Dealer *dealer);
int game_dealer_get_cards_in_hand(Dealer *dealer);
bool game_can_insure(Dealer *dealer);

bool game_player_bust(Player *player);
bool game_player_is_blackjack(Player *player);
bool game_player_can_hit(Player *player);
Card* game_player_hit(Deck *deck, Player *player);
Card** game_player_get_hand(Player *player);
int game_player_get_hand_value(Player *player);
int game_player_get_cards_in_hand(Player *player);
float game_player_get_money(Player *player);
float game_player_get_bet(Player *player);
bool game_player_is_bet_history_empty(Player *player);
bool game_player_is_bet_history_full(Player *player);
bool game_player_bet_push(Player *player, CHIP_VALUE val);
CHIP_VALUE game_player_bet_pop(Player *player);
CHIP_VALUE game_player_bet_peek(Player *player);
void game_player_add_money_to_bet(Player *player, float money);
void game_player_sub_money_from_bet(Player *player, float money);
void game_player_add_money(Player *player, float money);
void game_player_sub_money(Player *player, float money);
bool game_player_can_bet(Player *player);
bool game_player_can_double_down(Player *player);