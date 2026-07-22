#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_video.h>

#include <stdbool.h>
#include <stdlib.h>

#define STB_DS_IMPLEMENTATION
#include "../include/stb_ds.h"

#include "../include/constants.h"
#include "../include/card_constants.h"
#include "../include/main.h"
#include "../include/widget.h"
#include "../include/picbox.h"
#include "../include/container.h"
#include "../include/spritebox.h"
#include "../include/button.h"
#include "../include/label.h"
#include "../include/render.h"
#include "../include/input.h"
#include "../include/update.h"
/*
#include "../include/update.h"
#include "../include/animate.h"
#include "../include/render.h"
#include "../include/labels.h"
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


Container* widgets_initialize(InputContext* p_ic, ButtonContext *p_bc, render_hash* texture_map){
    Container *root = container_create(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, true, NULL);
    
    PictureBox *bkg = picturebox_create(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, true,
        hmget(texture_map, 1), NULL
    );
    container_add_widget(root, (Widget*)bkg);
    
    Container *cards = container_create(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, true, NULL);
    for (int i = 0; i < 52; i++){
        SpriteBox *card = spritebox_create(DECK_X_ORIGIN-(52-i), DECK_Y_ORIGIN+(52-i), CARD_WIDTH, CARD_HEIGHT, true,
            hmget(texture_map, 2), 0, (CARD_HEIGHT+2)*4, NULL);
        container_add_widget(cards, (Widget *)card);
    }
    container_add_widget(root, (Widget *)cards);
    
    Container *action_buttons = container_create(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, true, NULL);
    
    Button *deal_button = button_create(
        ACTION_BUTTON_X_ORIGIN, ACTION_BUTTON_Y_ORIGIN, ACTION_BUTTON_WIDTH, ACTION_BUTTON_HEIGHT, 
        true,
        event_app_event_create(BUTTON_EVENT_RELEASE_DEAL),
        BUTTON_STATE_IDLE, 
        hmget(texture_map, 3), 
        button_callback_deal,
        button_update_deal
    );
    container_add_widget(action_buttons, (Widget *)deal_button);
    input_subscriber_add(p_ic, (Widget *)deal_button);
    button_context_add_dynamically_positioned_button(p_bc, deal_button);

    Button *hit_button = button_create(
        ACTION_BUTTON_X_ORIGIN, ACTION_BUTTON_Y_ORIGIN, ACTION_BUTTON_WIDTH, ACTION_BUTTON_HEIGHT,
        true, 
        event_app_event_create(BUTTON_EVENT_RELEASE_HIT), 
        BUTTON_STATE_IDLE,
        hmget(texture_map, 4), 
        button_callback_hit,
        button_update_hit
    );
    container_add_widget(action_buttons, (Widget *)hit_button);
    input_subscriber_add(p_ic, (Widget *)hit_button);
    button_context_add_dynamically_positioned_button(p_bc, hit_button);

    button_add_subscriber(hit_button, (Widget *)deal_button);
    button_add_subscriber(deal_button, (Widget *)hit_button);

    container_add_widget(root, (Widget *)action_buttons);

    return root;
}

void widgets_teardown(Container *root){
    Widget** children = container_get_children(root);
    for (int i = 0; i < arrlen(root->children); i++){
        switch(children[i]->wtype){
            case WIDGET_CONTAINER:
                widgets_teardown((Container *)children[i]);
                break;
            case WIDGET_PICBOX:
                picturebox_destroy((PictureBox *)children[i]);
                break;
            case WIDGET_BUTTON:
                button_destroy((Button *)children[i]);
                break;
            case WIDGET_LABEL:
                label_destroy((Label *)children[i]);
                break;
            case WIDGET_SPRITEBOX:
                spritebox_destroy((SpriteBox *)children[i]);
                break;
        }
        children[i] = NULL;
    }
    container_destroy(root);
    root = NULL;
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
    InputContext *p_ic = input_initialize();
    ButtonContext* p_bc = button_context_initialize();
    render_hash* texture_map = texture_map_create(p_as->renderer);
    p_as->root = widgets_initialize(p_ic, p_bc, texture_map);
    UpdateContext *p_uc = update_context_initialize();
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
        should_quit = handle_input(p_ic);
        render(p_as->renderer, p_as->root);
        update(p_uc, p_bc);
    }
    update_context_teardown(p_uc);
    widgets_teardown(p_as->root);
    texture_map_destroy(texture_map);
    button_context_teardown(p_bc);
    input_teardown(p_ic);
    SDL_DestroyRenderer(p_as->renderer);
    SDL_DestroyWindow(p_as->window);
    free(p_as);
    SDL_Quit();
}