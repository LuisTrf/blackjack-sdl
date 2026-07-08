#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_video.h>

#include <stdbool.h>

#include "../include/game_constants.h"
#include "../include/input.h"
#include "../include/update.h"
#include "../include/animate.h"
#include "../include/render.h"
#include "../include/game.h"
#include "../include/labels.h"
#include "../include/events.h"

static SDL_Window *window;
static SDL_Renderer *renderer;

bool initialize_window(void){
    if (!SDL_Init(SDL_INIT_VIDEO)){
        SDL_Log("Error initializing SDL: %s\n", SDL_GetError());
        return false;
    }
    window = SDL_CreateWindow(
        "Blackjack",
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        0
    );
    if (!window){
        SDL_Log("Error initializing window: %s\n", SDL_GetError());
        return false;
    }
    SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
    SDL_SetWindowMinimumSize(window, WINDOW_WIDTH, WINDOW_HEIGHT);
    renderer = SDL_CreateRenderer(
        window,
        NULL
    );
    if (!renderer){
        SDL_Log("Error initializing renderer: %s\n", SDL_GetError());
        return false;
    }
    /*
    UPDATE GAMEOBJ MEMBERS WHEN RESIZING.
    if (!SDL_SetRenderLogicalPresentation(renderer, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_LOGICAL_PRESENTATION_STRETCH)){
        SDL_Log("Error initializing logical presentation: %s\n", SDL_GetError());
        return false;
    };
    */
    if (!TTF_Init()){
        SDL_Log("Error initializing SDL_ttf: %s\n", SDL_GetError());
        return false;
    }
    return true;
}

void allocate_memory(void){
    allocate_deck_memory();
    initialize_anim_queue();
    initialize_anim_pool();
    initialize_event_queue();
}

void teardown(void){
    resource_teardown();
    free_allocated_anim_pool_memory();
    free_allocated_anim_queue_memory();
    free_deck_memory();
    TTF_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

int main(int argc, char **argv){
    bool running = initialize_window();
    allocate_memory();
    get_renderer(renderer);
    load_resources();
    update_all_label_dimensions();
    while (running){
        running=handle_input();
        update();
        animate();
        poll_events();
        render();
    }
    teardown();
}