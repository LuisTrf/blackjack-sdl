#pragma once

#include "game_constants.h"
#include "gameobj.h"
#include "game.h"

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