#ifndef RENDER_H
#define RENDER_H

#include <SDL3/SDL.h>
#include "card_constants.h"
#include "buttons.h"

void get_renderer(SDL_Renderer *sdl_renderer);
void load_resources(void);
void render(void);
void resource_teardown(void);

#define SPRITESHEET_SEP 2

#define BUTTON_SPRITESHEET_IDLE_REL_OFFSET_X 0
#define BUTTON_SPRITESHEET_IDLE_REL_OFFSET_Y 0
#define BUTTON_SPRITESHEET_DISABLED_REL_OFFSET_X 1
#define BUTTON_SPRITESHEET_DISABLED_REL_OFFSET_Y 0
#define BUTTON_SPRITESHEET_HOVERED_REL_OFFSET_X 0
#define BUTTON_SPRITESHEET_HOVERED_REL_OFFSET_Y 1
#define BUTTON_SPRITESHEET_PRESSED_REL_OFFSET_X 1
#define BUTTON_SPRITESHEET_PRESSED_REL_OFFSET_Y 1

#endif