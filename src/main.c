#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_video.h>

#include <stdbool.h>
#include <stdlib.h>

#define STB_DS_IMPLEMENTATION
#include "../include/stb_ds.h"

#include "../include/constants.h"
#include "../include/main.h"

App_State* app_state_create(void){
    App_State as;
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
    as.ic = input_context_create();
    as.uc = update_context_create();
    as.texture_map = texture_map_create(as.renderer);
    as.gc = game_context_create();
    as.ec = event_context_create();
    as.ac = animation_context_create();
    as.uic = ui_context_create(as.texture_map, as.ic, as.ec);
    as.should_quit = false;
    App_State *p_as = malloc(sizeof(App_State));
    if (p_as == NULL){
        abort();
    }
    *p_as = as;
    return p_as;
}

void app_state_destroy(App_State *as){
    ui_context_destroy(as->uic);
    animation_context_destroy(as->ac);
    event_context_destroy(as->ec);
    game_context_destroy(as->gc);
    texture_map_destroy(as->texture_map);
    update_context_destroy(as->uc);
    input_context_destroy(as->ic);
    SDL_DestroyRenderer(as->renderer);
    SDL_DestroyWindow(as->window);
    free(as);
}

int main(int argc, char **argv){
    App_State *as = app_state_create();
    while (!as->should_quit){
        as->should_quit = input_handle(as);
        update(as);
        render(as);
    }
    app_state_destroy(as);
    SDL_Quit();
}