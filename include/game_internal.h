#pragma once

#include "game.h"

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
    Player *player;
    Dealer *dealer;
} GameContext;