#pragma once

#include <stdbool.h>
#include "widget.h"
#include "../event/event_types.h"
#include "../render/render_types.h"

typedef struct Label {
    Widget widget;
    FONT_ID fid;
    TEXTURE_ID tid;
    bool _retex;
    char txt[64];
} Label;

Label* label_create(
    font_hash* font_map,
    float x, float y, 
    bool visible, 
    FONT_ID fid,
    TEXTURE_ID tid
);
void label_destroy(Label *label);

void label_write(Label *label, font_hash* font_map, const char *fmt, ...);

void label_notify_dealer_hand(void *self, Event event, void *dependencies);
void label_notify_player_hand(void *self, Event event, void *dependencies);
void label_notify_player_money(void *self, Event event, void *dependencies);
void label_notify_player_bet(void *self, Event event, void *dependencies);
void label_notify_player_split_hand(void *self, Event event, void *dependencies);