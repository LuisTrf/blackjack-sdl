#pragma once

#include <SDL3/SDL.h>
#include "input/input.h"
#include "update/update.h"
#include "render/render.h"
#include "game/game.h"
#include "event/event.h"
#include "update/animation.h"
#include "ui/ui.h"

typedef struct App_State {
    SDL_Window *window;
    SDL_Renderer *renderer;
    Input_Context *ic;
    Update_Context *uc;
    Render_Context *rc;
    Game_Context *gc;
    Event_Context *ec;
    Animation_Context *ac;
    UI_Context *uic;
} App_State;