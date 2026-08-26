#pragma once

#include <SDL3/SDL.h>
#include <SDL3/SDL_stdinc.h>
#include "input/input_listener.h"
#include "render/render_types.h"
#include "game/game.h"
#include "event/event.h"
#include "update/animation.h"
#include "ui/container.h"
#include "ui/ui.h"

typedef struct AppState {
    SDL_Window *window;
    bool should_quit;
    float delta_time;
    Uint64 prev_frametime;
    SDL_Renderer *renderer;
    font_hash* font_map;
    texture_hash* texture_map;
    InputListener **p_input_listeners;
    EventQueue *event_queue;
    EventListener **p_event_listeners;
    GameContext *game_ctx;
    AnimationQueue *anim_queue;
    AnimationPool *anim_pool;
    Container* ui_root;
} AppState;