#pragma once

#include <stdbool.h>

#define PLAYER_MAXIMUM_HAND_SIZE 5
#define DEALER_MAXIMUM_HAND_SIZE 10
#define MAXIMUM_BETTED_CHIPS 256

typedef enum {
    CARD_LOCATION_DECK,
    CARD_LOCATION_PLAYER_HAND,
    CARD_LOCATION_DEALER_HAND
} CARD_LOCATION;

typedef enum {
    _GAME_STATE_NONE,
    GAME_STATE_NEW,
    GAME_STATE_BETTING,
    GAME_STATE_PLAYING
} GAME_STATE;

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

typedef struct Player {
    Card *hand[PLAYER_MAXIMUM_HAND_SIZE];
    unsigned char hand_value;
    unsigned char cards_in_hand;
    unsigned char aces_in_hand_worth_11;
    float money;
    float bet;
    CHIP_VALUE bet_history[MAXIMUM_BETTED_CHIPS];
    int betted_chips;
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
    Card** deck;
    struct Player *player;
    struct Dealer *dealer;
} GameContext;

#define PLAYER_BEGINNING_MONEY 100000.00
#define STANDARD_BET_PAYOUT 2.f
#define PLAYER_BLACKJACK_BET_PAYOUT 1.5f

extern const char suits[4];
extern const char ranks[13];
extern const int rank_values[13];

GameContext* game_context_create(void);
void game_context_destroy(GameContext *p_gc);

GAME_STATE game_context_get_game_state(GameContext *p_gc);
GAME_STATE game_context_get_prev_game_state(GameContext *p_gc);
void game_context_set_game_state(GameContext *p_gc, GAME_STATE state);
void game_shuffle_deck(Card **deck);
char game_get_card_suit(Card *card);
char game_get_card_rank(Card *card);
CARD_LOCATION game_get_card_location(Card *card);
Card* game_draw_random_card(Card **deck);
void game_dealer_reset(struct Dealer *dealer);
void game_player_reset(struct Player *player);
void game_participants_reset(struct Dealer *dealer, struct Player *player);
bool game_dealer_bust(struct Dealer *dealer);
bool game_player_bust(struct Player *player);
bool game_dealer_is_blackjack(struct Dealer *dealer);
bool game_player_is_blackjack(struct Player *player);
bool game_dealer_can_hit(struct Dealer *dealer);
bool game_player_can_hit(struct Player *player);
bool game_dealer_is_hiding_second_card(struct Dealer *dealer);
void game_dealer_reveal_second_card(struct Dealer *dealer);
Card* game_dealer_hit(Card** deck, struct Dealer *dealer);
Card* game_player_hit(Card **deck, struct Player *player);
Card** game_dealer_get_hand(struct Dealer *dealer);
Card** game_player_get_hand(struct Player *player);
int game_dealer_get_hand_value(struct Dealer *dealer);
int game_player_get_hand_value(struct Player *player);
int game_player_get_cards_in_hand(struct Player *player);
int game_dealer_get_cards_in_hand(struct Dealer *dealer);
float game_player_get_money(struct Player *player);
float game_player_get_bet(struct Player *player);
bool game_player_is_bet_history_empty(struct Player *player);
bool game_player_is_bet_history_full(struct Player *player);
bool game_player_bet_push(struct Player *player, CHIP_VALUE val);
CHIP_VALUE game_player_bet_pop(struct Player *player);
CHIP_VALUE game_player_bet_peek(struct Player *player);
void game_player_add_money_to_bet(struct Player *player, float money);
void game_player_sub_money_from_bet(struct Player *player, float money);
void game_player_add_money(struct Player *player, float money);
void game_player_sub_money(struct Player *player, float money);
bool game_player_can_bet(struct Player *player);
bool game_player_can_double_down(struct Player *player);
bool game_can_insure(struct Dealer *dealer);