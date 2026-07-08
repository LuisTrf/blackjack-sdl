#ifndef LABELS_H
#define LABELS_H

#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3/SDL.h>
#include "game_object.h"

typedef struct Label {
    GameObject obj;
    TTF_Font *font;
    SDL_Texture *texture;
    float font_size;
    char txt[32];
} Label;

#define STRING(s) #s
#define XSTRING(s) STRING(s)
#define DOLLAR_STRING(s) "$" XSTRING(s)

#define FONT_SIZE 28.0f

#define PLAYER_MONEY_LABEL_X 0
#define PLAYER_MONEY_LABEL_Y 0

#define HAND_FONT_LABEL_X_OFFSET_FROM_HAND 50

#define PLAYER_BET_LABEL_Y 20

#define NUMBER_OF_LABELS 4

extern Label player_money_label;
extern Label dealer_hand_label;
extern Label player_hand_label;
extern Label player_bet_label;

extern Label* labels[NUMBER_OF_LABELS];

void update_label_dimensions(Label *label);
void align_label_x(Label *label, float target_x);
void align_label_y(Label *label, float target_y);
void update_all_label_dimensions(void);

#endif