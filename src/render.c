#include <SDL3/SDL.h>
#include <SDL3/SDL_pixels.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_surface.h>

#include "../include/stb_ds.h"
#include "../include/widget.h"
#include "../include/container.h"
#include "../include/picbox.h"
#include "../include/spritebox.h"
#include "../include/button.h"
#include "../include/render.h"

SDL_Texture* render_load_texture_from_png(SDL_Renderer *renderer, char* filepath){
    SDL_Surface *p_surface = SDL_LoadPNG(filepath);
    SDL_Texture *p_texture = SDL_CreateTextureFromSurface(renderer, p_surface);
    SDL_DestroySurface(p_surface);
    return p_texture;
}

render_hash* texture_map_create(SDL_Renderer *renderer){
    render_hash* texture_map = NULL;
    hmput(texture_map, 1, render_load_texture_from_png(renderer, "../resources/bg.png"));
    hmput(texture_map, 2, render_load_texture_from_png(renderer, "../resources/cards.png"));
    hmput(texture_map, 3, render_load_texture_from_png(renderer, "../resources/deal_spritesheet.png"));
    hmput(texture_map, 4, render_load_texture_from_png(renderer, "../resources/hit_spritesheet.png"));
    return texture_map;
}

void texture_map_destroy(render_hash* texture_map){
    for (int i = 0; i < hmlen(texture_map); i++){
        SDL_DestroyTexture(texture_map[i].value);
        texture_map[i].value = NULL;
    }
    hmfree(texture_map);
}

void render_picbox(SDL_Renderer *renderer, PictureBox *picbox){
    SDL_FRect dst_rect = {
        picbox->widget.pos.x,
        picbox->widget.pos.y,
        picbox->widget.width,
        picbox->widget.height
    };
    SDL_RenderTexture(renderer, picbox->p_texture, NULL, &dst_rect);
}

void render_spritebox(SDL_Renderer *renderer, SpriteBox *spritebox){
    SDL_FRect src_rect = {
        spritebox->spritesheet_x,
        spritebox->spritesheet_y,
        spritebox->widget.width,
        spritebox->widget.height
    };
    SDL_FRect dst_rect = {
        spritebox->widget.pos.x,
        spritebox->widget.pos.y,
        spritebox->widget.width,
        spritebox->widget.height
    };
    SDL_RenderTexture(renderer, spritebox->p_spritesheet, &src_rect, &dst_rect);
}

void render_button(SDL_Renderer *renderer, Button *button){
    if (!button->widget.visible || button->p_spritesheet==NULL) {return;}
    int rel_offset_x, rel_offset_y;
    SDL_FRect dst_rect = {
        button->widget.pos.x,
        button->widget.pos.y,
        button->widget.width,
        button->widget.height
    };
    switch (button_get_state(button)) {
        case (_BUTTON_STATE_NONE):
            break;
        case (BUTTON_STATE_IDLE):
            rel_offset_x=0, rel_offset_y=0;
            break;
        case (BUTTON_STATE_HOVERED): 
            rel_offset_x=0, rel_offset_y=1;
            break;
        case (BUTTON_STATE_PRESSED):
            rel_offset_x=1, rel_offset_y=1;
            break;
        case (BUTTON_STATE_DISABLED):
            rel_offset_x=1, rel_offset_y=0;
            break;
    }
    SDL_FRect src_rect = {
        (rel_offset_x)*(button->widget.width+2),
        (rel_offset_y)*(button->widget.height+2),
        button->widget.width,
        button->widget.height
    };
    SDL_RenderTexture(renderer, button->p_spritesheet, &src_rect, &dst_rect);
}

void render_widgets(SDL_Renderer *renderer, Container *root){
    Widget** children = container_get_children(root);
    for (int i = 0; i < arrlen(children); i++){
        if (children[i] == NULL) {return;}
        switch(children[i]->wtype){
            case WIDGET_CONTAINER:
                render_widgets(renderer, (Container *)children[i]);
                break;
            case WIDGET_PICBOX:
                render_picbox(renderer, (PictureBox *)children[i]);
                break;
            case WIDGET_SPRITEBOX:
                render_spritebox(renderer, (SpriteBox *)children[i]);
                break;
            case WIDGET_BUTTON:
                render_button(renderer, (Button *)children[i]);
                break;
            default:
                break;
        }
    }
}

void render(SDL_Renderer *renderer, Container *root){
    render_widgets(renderer, root);
    SDL_RenderPresent(renderer);
}

/*
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
    Using src_rect to not render spritesheet is a hacky solution think of something better.
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
*/