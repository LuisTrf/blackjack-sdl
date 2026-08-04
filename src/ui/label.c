#include <SDL3_ttf/SDL_ttf.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

#include "../../include/stb_ds.h"
#include "../../include/ui/label.h"

/*
Label player_money_label = {{PLAYER_MONEY_LABEL_X, PLAYER_MONEY_LABEL_Y, 0, 0, true}, NULL, NULL, FONT_SIZE, 
    DOLLAR_STRING(PLAYER_BEGINNING_MONEY)};
Label dealer_hand_label = {{0, 0, 0, 0, true}, NULL, NULL, FONT_SIZE, ""};
Label player_hand_label = {{0, 0, 0, 0, true}, NULL, NULL, FONT_SIZE, ""};
Label player_bet_label = {{0, PLAYER_BET_LABEL_Y, 0, 0, false}, NULL, NULL, FONT_SIZE, ""};
*/

void label_update_dimensions(Label *label, font_hash* font_map){
    TTF_GetStringSize(
        hmget(font_map, label->fid), 
        label->txt, 
        0, 
        &(label->widget.rect.width), 
        &(label->widget.rect.height)
    );
}

Label* label_create(
    font_hash* font_map,
    float x, float y, 
    bool visible, 
    FONT_ID fid,
    TEXTURE_ID tid
)
{
    Label label = {
        {
            {
                .pos={.x=x, .y=y}, 
                .visible=visible, 
            },
            WIDGET_LABEL, 
        }, 
        fid,
        tid, 
        false,
        ""
    };
    Label* p_label = malloc(sizeof(Label));
    if (p_label==NULL){
        abort();
    }
    *p_label = label;
    label_update_dimensions(p_label, font_map);
    return p_label;
}

void label_destroy(Label *p_label){
    free(p_label);
    p_label=NULL;
}

void label_write(Label *label, font_hash* font_map, const char *fmt, ...){
    va_list args;
    va_start(args, fmt);
    vsnprintf(label->txt, sizeof(label->txt)/sizeof(char), fmt, args);
    va_end(args);
    printf("%s\n", label->txt);
    label_update_dimensions(label, font_map);
    label->_retex = true;
}

void label_align_x(Label *label, float target_x){
    label->widget.rect.pos.x = target_x - (label->widget.rect.width)/2.f;
}

void label_align_y(Label *label, float target_y){
    label->widget.rect.pos.y = target_y - (label->widget.rect.height)/2.f;
}