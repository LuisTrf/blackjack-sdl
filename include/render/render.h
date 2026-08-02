#pragma once

#include "render_types.h"
#include "../app_state.h"

font_hash* font_map_create(void);
void font_map_destroy(font_hash* font_map);
texture_hash* texture_map_create(SDL_Renderer *renderer, font_hash* font_map);
void texture_map_destroy(texture_hash* texture_map);
void render(AppState *as);