#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_video.h>

#include <stdbool.h>
#include <stdlib.h>

#define STB_DS_IMPLEMENTATION
#include "../include/stb_ds.h"

#include "../include/constants.h"
#include "../include/main_internal.h"
/*
#include "../include/update.h"
#include "../include/animate.h"
#include "../include/render.h"
#include "../include/labels.h"
*/

bool app_state_create(AppState *as){
    if (!SDL_Init(SDL_INIT_VIDEO)){
        SDL_Log("Error initializing SDL: %s\n", SDL_GetError());
        exit(1);
    }
    as->window = SDL_CreateWindow(
        "Blackjack",
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        0
    );
    if (!as->window){
        SDL_Log("Error initializing window: %s\n", SDL_GetError());
        exit(1);
    }
    SDL_SetWindowPosition(as->window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
    SDL_SetWindowMinimumSize(as->window, WINDOW_WIDTH, WINDOW_HEIGHT);
    as->renderer = SDL_CreateRenderer(
        as->window,
        NULL
    );
    if (!as->renderer){
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
    return false;
}

int main(int argc, char **argv){
    App_State as = {NULL, NULL, NULL};
    bool should_quit = app_state_initialize(&as);
    AppState *p_as = malloc(sizeof(AppState));
    if (p_as == NULL){
        abort();
    }
    *p_as = as;
    UpdateContext *p_uc = update_context_create();
    EventContext *p_ec = event_context_create();
    AnimationContext *p_ac = animation_context_create();
    GameContext *p_gc = game_context_create();
    InputContext *p_ic = input_context_create();
    ButtonContext* p_bc = button_context_create();
    render_hash* texture_map = texture_map_create(p_as->renderer);
    p_as->root = ui_root_initialize(p_ic, p_ec, p_bc, texture_map);
    while (!should_quit){
        should_quit = input_handle(p_ic, p_ec->queue);
        update(p_uc, p_gc, p_ec, p_ac, p_bc);
        render(p_as->renderer, texture_map, p_as->root, p_gc);
    }
    widgets_teardown(p_as->root);
    texture_map_destroy(texture_map);
    button_context_destroy(p_bc);
    animation_context_destroy(p_ac);
    event_context_destroy(p_ec);
    input_context_destroy(p_ic);
    update_context_destroy(p_uc);
    SDL_DestroyRenderer(p_as->renderer);
    SDL_DestroyWindow(p_as->window);
    free(p_as);
    SDL_Quit();
}