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

Container* widgets_initialize(render_hash* texture_map){
    Container *root = container_create(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, true, NULL);
    PictureBox *bkg = picturebox_create(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, true,
        hmget(texture_map, 1), NULL
    );
    container_add_widget(root, (Widget*)bkg);
    return root;
}

void widgets_teardown(Container *root){
    picturebox_destroy((PictureBox *)root->children[0]);
    container_destroy(root);
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
    render_hash* texture_map = texture_map_create(p_as->renderer);
    p_as->root = widgets_initialize(texture_map);
    /*
    allocate_memory();
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
        render(p_as->renderer, p_as->root);
        SDL_Delay(100);
    }
    widgets_teardown(p_as->root);
    texture_map_destroy(texture_map);
    SDL_DestroyRenderer(p_as->renderer);
    SDL_DestroyWindow(p_as->window);
    SDL_Quit();
}