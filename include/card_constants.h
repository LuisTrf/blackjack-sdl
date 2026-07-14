#pragma once

#include "../include/constants.h"

#define CARD_WIDTH 150
#define CARD_HEIGHT 210
#define CARD_STEP_X 25
#define CARD_X_ORIGIN (WINDOW_WIDTH/2.f - CARD_WIDTH/2.f - CARD_STEP_X*2.f)
#define CARD_Y_ORIGIN_DEALER 60
#define CARD_Y_ORIGIN_PLAYER 300
#define DECK_X_ORIGIN 120
#define DECK_Y_ORIGIN (CARD_Y_ORIGIN_DEALER/2.f + CARD_Y_ORIGIN_PLAYER/2.f)