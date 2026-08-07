#pragma once

#include "app_state.h"

#include <SDL3/SDL.h>
#include "input/input.h"
#include "update/update.h"
#include "render/render.h"
#include "game/game.h"
#include "event/event.h"
#include "update/animation.h"
#include "ui/container.h"
#include "ui/ui.h"

typedef struct AppState {
    SDL_Window *window;
    SDL_Renderer *renderer;
    InputContext *input_ctx;
    UpdateContext *update_ctx;
    font_hash* font_map;
    texture_hash* texture_map;
    GameContext *game_ctx;
    EventContext *event_ctx;
    AnimationContext *anim_ctx;
    Container* ui_root;
    bool should_quit;
} AppState;