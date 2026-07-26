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

PictureBox* widgets_initialize_bkg(render_hash* texture_map){
    PictureBox *bkg = picturebox_create(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, true,
        hmget(texture_map, TEXTURE_ID_BACKGROUND), NULL
    );
    return bkg;
}

Container* widgets_initialize_cards(render_hash* texture_map){
    Container *cards = container_create(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, true, NULL);
    for (int i = 0; i < 52; i++){
        SpriteBox *card = spritebox_create(DECK_X_ORIGIN-(52-i), DECK_Y_ORIGIN+(52-i), CARD_WIDTH, CARD_HEIGHT, true,
            hmget(texture_map, TEXTURE_ID_CARD_SPRITESHEET), 0, (CARD_HEIGHT+2)*4, NULL);
        container_add_widget(cards, (Widget *)card);
    }
    return cards;
}

Button* widgets_initialize_deal_button(InputContext* p_ic, ButtonContext *p_bc, render_hash* texture_map){
    Button *deal_button = button_create(
        ACTION_BUTTON_X_ORIGIN, ACTION_BUTTON_Y_ORIGIN, ACTION_BUTTON_WIDTH, ACTION_BUTTON_HEIGHT, 
        true,
        eventtyped_app_event_create(BUTTON_EVENT_RELEASE_DEAL),
        BUTTON_STATE_IDLE, 
        hmget(texture_map, TEXTURE_ID_DEAL_BUTTON_SPRITESHEET), 
        button_callback_deal,
        button_update_deal
    );
    input_context_widget_listener_add(p_ic, (Widget *)deal_button);
    button_context_add_dynamically_positioned_button(p_bc, deal_button);
    return deal_button;
}

Button* widgets_initialize_hit_button(InputContext* p_ic, ButtonContext *p_bc, render_hash* texture_map){
    Button *hit_button = button_create(
        ACTION_BUTTON_X_ORIGIN, ACTION_BUTTON_Y_ORIGIN, ACTION_BUTTON_WIDTH, ACTION_BUTTON_HEIGHT,
        true, 
        eventtyped_app_event_create(BUTTON_EVENT_RELEASE_HIT), 
        BUTTON_STATE_IDLE,
        hmget(texture_map, TEXTURE_ID_HIT_BUTTON_SPRITESHEET), 
        button_callback_hit,
        button_update_hit
    );
    input_context_widget_listener_add(p_ic, (Widget *)hit_button);
    button_context_add_dynamically_positioned_button(p_bc, hit_button);
    return hit_button;
}

Container* widgets_initialize_buttons(InputContext* p_ic, ButtonContext *p_bc, render_hash* texture_map){
    Container *buttons = container_create(
        0, 0, WINDOW_WIDTH, WINDOW_HEIGHT,
        true,
        NULL
    );
    Button* deal_button = widgets_initialize_deal_button(p_ic, p_bc, texture_map);
    container_add_widget(buttons, (Widget *)deal_button);
    Button* hit_button = widgets_initialize_hit_button(p_ic, p_bc, texture_map);
    container_add_widget(buttons, (Widget *)hit_button);

    button_add_subscriber(deal_button, (Widget *)hit_button);
    button_add_subscriber(hit_button, (Widget *)deal_button);

    return buttons;
}

Container* widgets_initialize(InputContext* p_ic, ButtonContext *p_bc, render_hash* texture_map){
    Container *root = container_create(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, true, NULL);
    
    PictureBox *bkg = widgets_initialize_bkg(texture_map);
    container_add_widget(root, (Widget*)bkg);
    
    Container *cards = widgets_initialize_cards(texture_map);
    container_add_widget(root, (Widget *)cards);
    
    Container *buttons = widgets_initialize_buttons(p_ic, p_bc, texture_map);
    container_add_widget(root, (Widget *)buttons);

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
    UpdateContext *p_uc = update_context_initialize();
    InputContext *p_ic = input_context_initialize();
    ButtonContext* p_bc = button_context_initialize();
    render_hash* texture_map = texture_map_create(p_as->renderer);
    p_as->root = widgets_initialize(p_ic, p_bc, texture_map);
    while (!should_quit){
        should_quit = handle_input(p_ic);
        update(p_uc, p_bc);
        render(p_as->renderer, p_as->root);
    }
    widgets_teardown(p_as->root);
    texture_map_destroy(texture_map);
    button_context_teardown(p_bc);
    input_context_teardown(p_ic);
    update_context_teardown(p_uc);
    SDL_DestroyRenderer(p_as->renderer);
    SDL_DestroyWindow(p_as->window);
    free(p_as);
    SDL_Quit();
}