#pragma once

#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3/SDL.h>
#include <stdbool.h>
#include "widget.h"
#include "../event/event_types.h"
#include "../render/render_types.h"

typedef struct Label {
    Widget widget;
    FONT_ID fid;
    TEXTURE_ID tid;
    bool _retex;
    char txt[32];
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
void label_align_x(Label *label, float target_x);
void label_align_y(Label *label, float target_y);

typedef struct label_notify_dependencies_t {
    font_hash* font_map;
} label_notify_dependencies;
void label_notify_dealer_hand(void *self, Event event, void *dependencies);
void label_notify_player_hand(void *self, Event event, void *dependencies);
void label_notify_player_money(void *self, Event event, void *dependencies);
void label_notify_player_bet(void *self, Event event, void *dependencies);