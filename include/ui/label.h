#pragma once

#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3/SDL.h>
#include <stdbool.h>
#include "widget.h"
#include "../event/event_types.h"

typedef struct Label {
    Widget widget;
    TTF_Font *p_font;
    SDL_Texture *p_texture;
    float font_size;
    char txt[32];
} Label;

Label* label_create(
    float x, float y, 
    int width, int height, 
    bool visible, 
    TTF_Font* font, float font_size, 
    Event (*notify_func)(Widget *self, Event event),
    Event (*input_func)(Widget *self, SDL_Event event)
);
void label_destroy(Label *p_label);

void label_update_dimensions(Label *label);
void label_align_x(Label *label, float target_x);
void label_align_y(Label *label, float target_y);
