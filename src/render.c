#include <SDL3/SDL.h>
#include <SDL3/SDL_pixels.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_surface.h>

#include <stdio.h>
#include <stdlib.h>

#include "../include/card_constants.h"
#include "../include/buttons.h"
#include "../include/game.h"
#include "../include/animate.h"
#include "../include/labels.h"
#include "../include/render.h"

static SDL_Renderer *renderer = NULL;
static SDL_Texture *background = NULL;
static SDL_Texture *card_spritesheet = NULL;
static TTF_Font *font = NULL;
static const SDL_Color FONT_COLOR = {255, 255, 255, 255};

void get_renderer(SDL_Renderer *sdl_renderer){
    renderer=sdl_renderer;
}

void load_background_texture(void){
    SDL_Surface *bkg_surface = SDL_LoadPNG("../resources/bg.png");
    background = SDL_CreateTextureFromSurface(renderer, bkg_surface);
    SDL_DestroySurface(bkg_surface);
}

void load_button_spritesheets(void){
    SDL_Surface *deal_button_spritesheet_surface = SDL_LoadPNG("../resources/deal_spritesheet.png");
    SDL_Surface *bet_button_spritesheet_surface = SDL_LoadPNG("../resources/bet_spritesheet.png");
    SDL_Surface *hit_button_spritesheet_surface = SDL_LoadPNG("../resources/hit_spritesheet.png");
    SDL_Surface *stand_button_spritesheet_surface = SDL_LoadPNG("../resources/stand_spritesheet.png");
    SDL_Surface *insurance_button_spritesheet_surface = SDL_LoadPNG("../resources/insurance_spritesheet.png");
    SDL_Surface *double_down_button_spritesheet_surface = SDL_LoadPNG("../resources/doubledown_spritesheet.png");

    deal_button.spritesheet = SDL_CreateTextureFromSurface(renderer, deal_button_spritesheet_surface);
    bet_button.spritesheet = SDL_CreateTextureFromSurface(renderer, bet_button_spritesheet_surface);
    hit_button.spritesheet = SDL_CreateTextureFromSurface(renderer, hit_button_spritesheet_surface);
    stand_button.spritesheet = SDL_CreateTextureFromSurface(renderer, stand_button_spritesheet_surface);
    insurance_button.spritesheet = SDL_CreateTextureFromSurface(renderer, insurance_button_spritesheet_surface);
    double_down_button.spritesheet = SDL_CreateTextureFromSurface(renderer, double_down_button_spritesheet_surface);

    SDL_DestroySurface(deal_button_spritesheet_surface);
    SDL_DestroySurface(bet_button_spritesheet_surface);
    SDL_DestroySurface(hit_button_spritesheet_surface);
    SDL_DestroySurface(stand_button_spritesheet_surface);
    SDL_DestroySurface(insurance_button_spritesheet_surface);
    SDL_DestroySurface(double_down_button_spritesheet_surface);

    SDL_Surface *white1_button_spritesheet_surface = SDL_LoadPNG("../resources/white1_spritesheet.png");
    SDL_Surface *red5_button_spritesheet_surface = SDL_LoadPNG("../resources/red5_spritesheet.png");
    SDL_Surface *blue10_button_spritesheet_surface = SDL_LoadPNG("../resources/blue10_spritesheet.png");
    SDL_Surface *green25_button_spritesheet_surface = SDL_LoadPNG("../resources/green25_spritesheet.png");
    SDL_Surface *black100_button_spritesheet_surface = SDL_LoadPNG("../resources/black100_spritesheet.png");
    SDL_Surface *purple500_button_spritesheet_surface = SDL_LoadPNG("../resources/purple500_spritesheet.png");
    SDL_Surface *yellow1k_button_spritesheet_surface = SDL_LoadPNG("../resources/yellow1k_spritesheet.png");
    SDL_Surface *orange5k_button_spritesheet_surface = SDL_LoadPNG("../resources/orange5k_spritesheet.png");
    SDL_Surface *redblue25k_button_spritesheet_surface = SDL_LoadPNG("../resources/redblue25k_spritesheet.png");
    SDL_Surface *gold100k_button_spritesheet_surface = SDL_LoadPNG("../resources/gold100k_spritesheet.png");
    
    white1_button.button.spritesheet = SDL_CreateTextureFromSurface(renderer, white1_button_spritesheet_surface);
    red5_button.button.spritesheet = SDL_CreateTextureFromSurface(renderer, red5_button_spritesheet_surface);
    blue10_button.button.spritesheet = SDL_CreateTextureFromSurface(renderer, blue10_button_spritesheet_surface);
    green25_button.button.spritesheet = SDL_CreateTextureFromSurface(renderer, green25_button_spritesheet_surface);
    black100_button.button.spritesheet = SDL_CreateTextureFromSurface(renderer, black100_button_spritesheet_surface);
    purple500_button.button.spritesheet = SDL_CreateTextureFromSurface(renderer, purple500_button_spritesheet_surface);
    yellow1k_button.button.spritesheet = SDL_CreateTextureFromSurface(renderer, yellow1k_button_spritesheet_surface);
    orange5k_button.button.spritesheet = SDL_CreateTextureFromSurface(renderer, orange5k_button_spritesheet_surface);
    redblue25k_button.button.spritesheet = SDL_CreateTextureFromSurface(renderer, redblue25k_button_spritesheet_surface);
    gold100k_button.button.spritesheet = SDL_CreateTextureFromSurface(renderer, gold100k_button_spritesheet_surface);

    SDL_DestroySurface(white1_button_spritesheet_surface);
    SDL_DestroySurface(red5_button_spritesheet_surface);
    SDL_DestroySurface(blue10_button_spritesheet_surface);
    SDL_DestroySurface(green25_button_spritesheet_surface);
    SDL_DestroySurface(black100_button_spritesheet_surface);
    SDL_DestroySurface(purple500_button_spritesheet_surface);
    SDL_DestroySurface(yellow1k_button_spritesheet_surface);
    SDL_DestroySurface(orange5k_button_spritesheet_surface);
    SDL_DestroySurface(redblue25k_button_spritesheet_surface);
    SDL_DestroySurface(gold100k_button_spritesheet_surface);
}

void load_card_spritesheet(void){
    SDL_Surface *card_spritesheet_surface = SDL_LoadPNG("../resources/cards.png");
    card_spritesheet = SDL_CreateTextureFromSurface(renderer, card_spritesheet_surface);
    SDL_DestroySurface(card_spritesheet_surface);
}

void load_font(void){
    font = TTF_OpenFont("../resources/OpenSans-VariableFont_wdth,wght.ttf", 32.0f);
    if (font==NULL){
        fprintf(stderr, "Failed to load font: %s", SDL_GetError());
    }
}

void create_font_texture(TTF_Font* font, const char* str, SDL_Texture** target_texture){
    SDL_Surface* font_surface = TTF_RenderText_Blended(font, str, 0, FONT_COLOR);
    SDL_DestroyTexture(*target_texture);
    *target_texture = SDL_CreateTextureFromSurface(renderer, font_surface);
    SDL_DestroySurface(font_surface);
}

void load_label_textures(void){
    for (int i=0; i<NUMBER_OF_LABELS; i++){
        labels[i]->font=font;
        create_font_texture(
            labels[i]->font,
            labels[i]->txt,
            &(labels[i]->texture)
        );
    }
}

void load_resources(void){
    load_background_texture();
    load_card_spritesheet();
    load_button_spritesheets();
    load_font();
    load_label_textures();
}

void render_button(Button* button, int button_width, int button_height, int button_spritesheet_step_x, int button_spritesheet_step_y){
    if (!button->obj.visible || button->spritesheet==NULL) {return;}
    int rel_offset_x, rel_offset_y;
    SDL_FRect button_rect = {
        button->obj.x,
        button->obj.y,
        button->obj.width,
        button->obj.height
    };
    switch (get_state(button)) {
        case (_NONE_BUTTON_STATE):
            break;
        case (IDLE):
            rel_offset_x=BUTTON_SPRITESHEET_IDLE_REL_OFFSET_X, rel_offset_y=BUTTON_SPRITESHEET_IDLE_REL_OFFSET_Y;
            break;
        case (HOVERED): 
        case (RELEASED):
            rel_offset_x=BUTTON_SPRITESHEET_HOVERED_REL_OFFSET_X, rel_offset_y=BUTTON_SPRITESHEET_HOVERED_REL_OFFSET_Y;
            break;
        case (PRESSED):
            rel_offset_x=BUTTON_SPRITESHEET_PRESSED_REL_OFFSET_X, rel_offset_y=BUTTON_SPRITESHEET_PRESSED_REL_OFFSET_Y;
            break;
        case (DISABLED):
            rel_offset_x=BUTTON_SPRITESHEET_DISABLED_REL_OFFSET_X, rel_offset_y=BUTTON_SPRITESHEET_DISABLED_REL_OFFSET_Y;
            break;
    }
    SDL_FRect src_rect = {
        rel_offset_x*button_spritesheet_step_x,
        rel_offset_y*button_spritesheet_step_y,
        button_width,
        button_height
    };
    SDL_RenderTexture(renderer, button->spritesheet, &src_rect, &button_rect);
}

void render_background(void){
    SDL_RenderTexture(renderer, background, NULL, NULL);
}

void render_deck(){
    SDL_FRect src_rect = {
        0,
        4*(CARD_HEIGHT+SPRITESHEET_SEP),
        CARD_WIDTH,
        CARD_HEIGHT
    };
    unsigned char visible_cards = 0;
    for (int i=(get_player_cards_in_hand()-1); i>=0; i--){
        if (get_player_hand()[i]->obj.visible) {visible_cards++;}
    }
    for (int i=(get_dealer_cards_in_hand()-1); i>=0; i--){
        if (get_dealer_hand()[i]->obj.visible) {visible_cards++;}
    }
    for (int i=52; i>visible_cards; i--){
        SDL_FRect deck_rect = {
            DECK_X_ORIGIN-i,
            DECK_Y_ORIGIN+i,
            CARD_WIDTH,
            CARD_HEIGHT
        };
        SDL_RenderTexture(renderer, card_spritesheet, &src_rect, &deck_rect);
    };
}

void render_card(Card *card){
    if (!card->obj.visible) {return;}
    SDL_FRect card_sdl_frect = {
        card->obj.x,
        card->obj.y,
        card->obj.width,
        card->obj.height
    };
    int rel_offset_x = 0, rel_offset_y = 0;
    if (card->face_down){
        rel_offset_x = 0;
        rel_offset_y = 4;
    }
    else {
        for (int i=0; i<13; i++){
            if (card->rank==ranks[i]){
                rel_offset_x = i;
                break;
            }
        }
        for (int j=0; j<4; j++){
            if (card->suit==suits[j]){
                rel_offset_y=j;
                break;
            }
        }
    }
    SDL_FRect src_rect = {
        rel_offset_x*(CARD_WIDTH+SPRITESHEET_SEP),
        rel_offset_y*(CARD_HEIGHT+SPRITESHEET_SEP),
        CARD_WIDTH,
        CARD_HEIGHT
    };
    SDL_RenderTexture(renderer, card_spritesheet, &src_rect, &card_sdl_frect);
}


void render_player_hand(void){
    Card** player_hand = get_player_hand();
    for (int i=0; i<get_player_cards_in_hand(); i++){
        render_card(player_hand[i]);
    }
}

void render_dealer_hand(void){
    Card** dealer_hand = get_dealer_hand();
    for (int i=0; i<get_dealer_cards_in_hand(); i++){
        render_card(dealer_hand[i]);
    }
}

void render_label(Label *label){
    if (!label->obj.visible) {return;}
    create_font_texture(
        label->font,
        label->txt,
        &(label->texture)
    );
    SDL_FRect label_rect = {
        label->obj.x,
        label->obj.y,
        label->obj.width,
        label->obj.height
    };
    SDL_RenderTexture(renderer, label->texture, NULL, &label_rect);
}

void render_textured_game_object(TexturedGameObject *textured_game_object){
    if (!textured_game_object->obj.visible) {return;}
    /*
    Using src_rect to not render spritesheet is a hacky solution think of something better.
    */
    SDL_FRect src_rect = {
        0.f,
        0.f,
        textured_game_object->obj.width,
        textured_game_object->obj.height
    };
    SDL_FRect textured_game_object_rect = {
        textured_game_object->obj.x,
        textured_game_object->obj.y,
        textured_game_object->obj.width,
        textured_game_object->obj.height
    };
    SDL_RenderTexture(renderer, textured_game_object->texture, &src_rect, &textured_game_object_rect);
}

void render_textured_game_objects(void){
    for (int i=0; i<MAXIMUM_ALIVE_TEXTURED_GAME_OBJS; i++){
        if (alive_textured_game_objects[i]!=NULL){
            render_textured_game_object(alive_textured_game_objects[i]);
        }
    }
}

void render_labels(void){
    render_label(&player_money_label);
    render_label(&player_bet_label);
    if (is_anim_queue_blocking()) {return;}
    render_label(&dealer_hand_label);
    render_label(&player_hand_label);
}

void render_buttons(void){
    if (is_anim_queue_blocking()) {return;}
    for (int i=0; i<NUMBER_OF_BUTTONS; i++){
        render_button(
            buttons[i], 
            buttons[i]->obj.width, 
            buttons[i]->obj.height, 
            buttons[i]->obj.width+SPRITESHEET_SEP, 
            buttons[i]->obj.height+SPRITESHEET_SEP
        );
    }
}

void render(void){
    SDL_RenderClear(renderer);
    render_background();
    render_buttons();
    render_deck();
    render_player_hand();
    render_dealer_hand();
    render_labels();
    render_textured_game_objects();
    SDL_RenderPresent(renderer);
}

void background_texture_teardown(void){
    SDL_DestroyTexture(background);
    background=NULL;
}

void button_texture_teardown(void){
    for (int i=0; i<NUMBER_OF_ACTION_BUTTONS; i++){
        SDL_DestroyTexture(action_buttons[i]->spritesheet);
        action_buttons[i]->spritesheet=NULL;
    }
    for (int i=0; i<NUMBER_OF_VALUED_BUTTONS; i++){
        SDL_DestroyTexture(valued_buttons[i]->button.spritesheet);
        valued_buttons[i]->button.spritesheet=NULL;
    }
}

void cards_texture_teardown(void){
    SDL_DestroyTexture(card_spritesheet);
    card_spritesheet=NULL;
}

void font_teardown(void){
    TTF_CloseFont(font);
    font=NULL;
}

void label_texture_teardown(void){
    SDL_DestroyTexture(player_money_label.texture);
    SDL_DestroyTexture(dealer_hand_label.texture);
    SDL_DestroyTexture(player_hand_label.texture);
    player_money_label.texture=NULL;
    dealer_hand_label.texture=NULL;
    player_hand_label.texture=NULL;
}

void resource_teardown(void){
    label_texture_teardown();
    font_teardown();
    button_texture_teardown();
    cards_texture_teardown();
    background_texture_teardown();
}