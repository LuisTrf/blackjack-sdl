#include <SDL3_ttf/SDL_ttf.h>
#include <stdlib.h>
#include "../../include/ui/label.h"

/*
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
*/

Label* label_create(
    float x, float y, 
    int width, int height, 
    bool visible, 
    TTF_Font* font, float font_size, 
    Event (*notify_func)(Widget *self, Event event),
    Event (*input_func)(Widget *self, SDL_Event event)
)
{
    Label label = {
        {
            {
                .pos={.x=x, .y=y}, 
                .width=width, 
                .height=height, 
                .visible=visible, 
            },
            WIDGET_LABEL, 
            notify_func,
            input_func
        }, 
        font, 
        NULL, 
        font_size, 
        ""
    };
    Label* p_label = malloc(sizeof(Label));
    if (p_label==NULL){
        abort();
    }
    *p_label = label;
    return p_label;
}

void label_destroy(Label *p_label){
    free(p_label);
    p_label=NULL;
}

void label_update_dimensions(Label *label){
    TTF_SetFontSize(label->font, label->font_size);
    TTF_GetStringSize(label->font, label->txt, 0, &(label->widget.rect.width), &(label->widget.rect.height));
}

void label_align_x(Label *label, float target_x){
    label->widget.rect.pos.x = target_x - (label->widget.rect.width)/2.f;
}

void label_align_y(Label *label, float target_y){
    label->widget.rect.pos.y = target_y - (label->widget.rect.height)/2.f;
}