#include <SDL3_ttf/SDL_ttf.h>
#include <stdio.h>
#include "../include/labels.h"
#include "../include/game.h"

Label player_money_label = {{PLAYER_MONEY_LABEL_X, PLAYER_MONEY_LABEL_Y, 0, 0, true}, NULL, NULL, FONT_SIZE, 
    DOLLAR_STRING(PLAYER_BEGINNING_MONEY)};
Label dealer_hand_label = {{0, 0, 0, 0, true}, NULL, NULL, FONT_SIZE, ""};
Label player_hand_label = {{0, 0, 0, 0, true}, NULL, NULL, FONT_SIZE, ""};
Label player_bet_label = {{0, PLAYER_BET_LABEL_Y, 0, 0, false}, NULL, NULL, FONT_SIZE, ""};
Label* labels[NUMBER_OF_LABELS] = {
    &player_money_label,
    &dealer_hand_label,
    &player_hand_label,
    &player_bet_label
};

void update_label_dimensions(Label *label){
    TTF_SetFontSize(label->font, label->font_size);
    TTF_GetStringSize(label->font, label->txt, 0, &(label->obj.width), &(label->obj.height));
}

void align_label_x(Label *label, float target_x){
    label->obj.x = target_x - (label->obj.width)/2.f;
}

void align_label_y(Label *label, float target_y){
    label->obj.y = target_y - (label->obj.height)/2.f;
}

void update_all_label_dimensions(void){
    for (int i=0; i<NUMBER_OF_LABELS; i++){
        update_label_dimensions(labels[i]);
    }
}