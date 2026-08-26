#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_video.h>

#include <stdbool.h>
#include <stdlib.h>

#define STB_DS_IMPLEMENTATION
#include "../include/stb_ds.h"

#include "../include/constants.h"
#include "../include/input/input.h"
#include "../include/render/render.h"
#include "../include/update/update.h"
#include "../include/main.h"

AppState* app_state_create(void){
    AppState as;
    if (!SDL_Init(SDL_INIT_VIDEO)){
        SDL_Log("Error initializing SDL: %s\n", SDL_GetError());
        exit(1);
    }
    as.window = SDL_CreateWindow(
        "Blackjack",
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        0
    );
    if (!as.window){
        SDL_Log("Error initializing window: %s\n", SDL_GetError());
        exit(1);
    }
    SDL_SetWindowPosition(as.window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
    SDL_SetWindowMinimumSize(as.window, WINDOW_WIDTH, WINDOW_HEIGHT);
    as.renderer = SDL_CreateRenderer(
        as.window,
        NULL
    );
    if (!as.renderer){
        SDL_Log("Error initializing renderer: %s\n", SDL_GetError());
        exit(1);
    }
    /*
    UPDATE WIDGETS WHEN RESIZING.
    if (!SDL_SetRenderLogicalPresentation(renderer, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_LOGICAL_PRESENTATION_STRETCH)){
        SDL_Log("Error initializing logical presentation: %s\n", SDL_GetError());
        return false;
    };
    */
    if (!TTF_Init()){
        SDL_Log("Error initializing SDL_ttf: %s\n", SDL_GetError());
        exit(1);
    }
    as.font_map = font_map_create();
    as.texture_map = texture_map_create(as.renderer, as.font_map);
    as.p_input_listeners = malloc(sizeof(InputListener*));
    if (as.p_input_listeners == NULL){
        abort();
    }
    *as.p_input_listeners = NULL;
    arrsetlen(*as.p_input_listeners, 0);
    as.prev_frametime = SDL_GetTicks();
    as.delta_time = 0.f;
    as.event_queue = event_queue_create(32);
    as.p_event_listeners = malloc(sizeof(EventListener*));
    if (as.p_event_listeners == NULL){
        abort();
    }
    *as.p_event_listeners = NULL;
    arrsetlen(*as.p_input_listeners, 0);
    as.game_ctx = game_context_create();
    as.anim_queue = anim_queue_create(16);
    as.anim_pool = anim_pool_create(ANIMATION_POOL_MAXIMUM_ANIMATIONS);
    as.ui_root = ui_root_initialize(as.p_input_listeners, as.p_event_listeners, as.font_map, as.anim_pool);
    as.should_quit = false;
    AppState *p_as = malloc(sizeof(AppState));
    if (p_as == NULL){
        abort();
    }
    *p_as = as;
    return p_as;
}

void app_state_destroy(AppState *as){
    widgets_teardown(as->ui_root);
    anim_pool_destroy(as->anim_pool);
    anim_queue_destroy(as->anim_queue);
    game_context_destroy(as->game_ctx);
    arrfree(*as->p_event_listeners);
    free(as->p_event_listeners);
    event_queue_destroy(as->event_queue);
    arrfree(*as->p_input_listeners);
    free(as->p_input_listeners);
    texture_map_destroy(as->texture_map);
    font_map_destroy(as->font_map);
    SDL_DestroyRenderer(as->renderer);
    SDL_DestroyWindow(as->window);
    free(as);
}

int main(int argc, char **argv){
    AppState *as = app_state_create();
    while (!as->should_quit){
        as->should_quit = input_handle(as);
        update(as);
        render(as);
    }
    app_state_destroy(as);
    SDL_Quit();
}