#pragma once

#include "../constants.h"
#include "../ui/button_constants.h"

#define MAXIMUM_HAND_SIZE 12
#define MAXIMUM_BETTED_CHIPS 256

#define PLAYER_BEGINNING_MONEY 100000.00
#define STANDARD_BET_PAYOUT 2.f
#define PLAYER_BLACKJACK_BET_PAYOUT 1.5f

#define MAXIMUM_ALIVE_CHEQUES 16
#define CHEQUE_POSITION_BETTING_X (WINDOW_WIDTH/2.f - CHIP_BUTTON_WIDTH/2.f)
#define CHEQUE_POSITION_BETTING_Y 358

extern const char SUITS[4];
extern const char RANKS[13];
extern const int RANK_VALUES[13];
