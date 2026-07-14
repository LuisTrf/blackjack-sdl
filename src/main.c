#include <SDL3/SDL.h>
#include <SDL3/SDL_stdinc.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_video.h>

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#define STB_DS_IMPLEMENTATION
#include "../include/stb_ds.h"

#include "../include/constants.h"
#include "../include/main.h"
#include "../include/widget.h"
#include "../include/picbox.h"
#include "../include/container.h"
#include "../include/render.h"
/*
#include "../include/input.h"
#include "../include/update.h"
#include "../include/animate.h"
#include "../include/render.h"
#include "../include/labels.h"
#include "../include/events.h"
*/

bool app_state_initialize(AppState *as){
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

void widgets_initialize(AppState *as, RenderState *rs){
    Container *root = container_create(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, true, NULL);
    PictureBox *bkg = picturebox_create(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, true,
        hmget(rs->texture_map, 1),
        NULL
    );
    container_add_widget(root, (Widget*)bkg);
}

void widgets_teardown(AppState *as){
    picturebox_destroy((PictureBox *)as->root->children[0]);
    container_destroy(as->root);
}

/*
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
*/

int main(int argc, char **argv){
    AppState as = {NULL, NULL, NULL};
    bool should_quit = app_state_initialize(&as);
    AppState *p_as = malloc(sizeof(AppState));
    if (p_as == NULL){
        abort();
    }
    *p_as = as;
    RenderState *rs = render_initialize(p_as->renderer);
    /*
    allocate_memory();
    get_renderer(renderer);
    load_resources();
    update_all_label_dimensions();
    while (!should_quit){
        running=handle_input();
        update();
        animate();
        poll_events();
        render();
    }
    teardown();
    */
    while (!should_quit){
        render(rs);
        SDL_Delay(100);
    }
    render_teardown(rs);
    SDL_DestroyRenderer(p_as->renderer);
    SDL_DestroyWindow(p_as->window);
    SDL_Quit();
}