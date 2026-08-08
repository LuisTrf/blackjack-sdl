#include <SDL3/SDL.h>
#include <SDL3/SDL_pixels.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_surface.h>
#include <stdio.h>

#include "../../include/stb_ds.h"
#include "../../include/ui/widget.h"
#include "../../include/ui/container.h"
#include "../../include/ui/picbox.h"
#include "../../include/ui/spritebox.h"
#include "../../include/ui/button.h"
#include "../../include/ui/label.h"
#include "../../include/game/game.h"
#include "../../include/game/card_constants.h"
#include "../../include/game/game_constants.h"
#include "../../include/render/render.h"
#include "../../include/main.h"

TTF_Font* render_load_font_from_ttf(char* filepath){
    TTF_Font *font = TTF_OpenFont(filepath, 32.0f);
    if (font == NULL){
        fprintf(stderr, "FAILED TO LOAD FONT: %s\n", SDL_GetError());
    }
    return font;
}

font_hash* font_map_create(void){
    font_hash* font_map = NULL;
    hmput(font_map, FONT_ID_OPENSANS_32PT, render_load_font_from_ttf("resources/font/OpenSans-VariableFont_wdth,wght.ttf"));
    hmput(font_map, FONT_ID_OPENSANS_ITALIC_32PT, render_load_font_from_ttf("resources/font/OpenSans-Italic-VariableFont_wdth,wght.ttf"));
    return font_map;
}

void font_map_destroy(font_hash* font_map){
    for (int i = 0; i < hmlen(font_map); i++){
        TTF_CloseFont(font_map[i].value);
        font_map[i].value = NULL;
    }
    hmfree(font_map);
}

SDL_Texture* render_load_texture_from_png(SDL_Renderer *renderer, char* filepath){
    SDL_Surface *surface = SDL_LoadPNG(filepath);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_DestroySurface(surface);
    return texture;
}

SDL_Texture* render_create_empty_font_texture(SDL_Renderer *renderer, font_hash* font_map, FONT_ID fid){
    SDL_Surface* surface = TTF_RenderText_Blended(hmget(font_map, fid), "", 0, FONT_COLOR);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_DestroySurface(surface);
    return texture;
}

void rerender_font_texture(SDL_Renderer *renderer, texture_hash* texture_map, font_hash* font_map, TEXTURE_ID tid, FONT_ID fid, const char* txt){
    SDL_Surface* surface = TTF_RenderText_Blended(hmget(font_map, fid), txt, 0, FONT_COLOR);
    SDL_DestroyTexture(hmget(texture_map, tid));
    hmput(texture_map, tid, NULL);
    hmput(texture_map, tid, SDL_CreateTextureFromSurface(renderer, surface));
    SDL_DestroySurface(surface);
}

texture_hash* texture_map_create(SDL_Renderer *renderer, font_hash* font_map){
    texture_hash* texture_map = NULL;
    hmput(texture_map, TEXTURE_ID_BACKGROUND, render_load_texture_from_png(renderer, "resources/bg.png"));
    hmput(texture_map, TEXTURE_ID_CARD_SPRITESHEET, render_load_texture_from_png(renderer, "resources/cards.png"));
    hmput(texture_map, TEXTURE_ID_DEAL_BUTTON_SPRITESHEET, render_load_texture_from_png(renderer, "resources/button/deal_spritesheet.png"));
    hmput(texture_map, TEXTURE_ID_HIT_BUTTON_SPRITESHEET, render_load_texture_from_png(renderer, "resources/button/hit_spritesheet.png"));
    hmput(texture_map, TEXTURE_ID_STAND_BUTTON_SPRITESHEET, render_load_texture_from_png(renderer, "resources/button/stand_spritesheet.png"));
    hmput(texture_map, TEXTURE_ID_BET_BUTTON_SPRITESHEET, render_load_texture_from_png(renderer, "resources/button/bet_spritesheet.png"));
    hmput(texture_map, TEXTURE_ID_WHITE_BUTTON_SPRITESHEET, render_load_texture_from_png(renderer, "resources/button/white1_spritesheet.png"));
    hmput(texture_map, TEXTURE_ID_LABEL_DEALER_HAND, render_create_empty_font_texture(renderer, font_map, FONT_ID_OPENSANS_32PT));
    hmput(texture_map, TEXTURE_ID_LABEL_PLAYER_HAND, render_create_empty_font_texture(renderer, font_map, FONT_ID_OPENSANS_32PT));
    hmput(texture_map, TEXTURE_ID_LABEL_PLAYER_MONEY, render_create_empty_font_texture(renderer, font_map, FONT_ID_OPENSANS_32PT));
    return texture_map;
}

void texture_map_destroy(texture_hash* texture_map){
    for (int i = 0; i < hmlen(texture_map); i++){
        SDL_DestroyTexture(texture_map[i].value);
        texture_map[i].value = NULL;
    }
    hmfree(texture_map);
}

void render_picturebox(SDL_Renderer *renderer, texture_hash* texture_map, PictureBox *picturebox){
    if (!picturebox->widget.rect.visible) {return;}
    SDL_FRect dst_rect = {
        picturebox->widget.rect.pos.x,
        picturebox->widget.rect.pos.y,
        picturebox->widget.rect.width,
        picturebox->widget.rect.height
    };
    SDL_RenderTexture(renderer, hmget(texture_map, picturebox->tid), NULL, &dst_rect);
}

void render_spritebox(SDL_Renderer *renderer, texture_hash *texture_map, SpriteBox *spritebox){
    if (!spritebox->widget.rect.visible) {return;}
    SDL_FRect src_rect = {
        spritebox->spritesheet_x,
        spritebox->spritesheet_y,
        spritebox->widget.rect.width,
        spritebox->widget.rect.height
    };
    SDL_FRect dst_rect = {
        spritebox->widget.rect.pos.x,
        spritebox->widget.rect.pos.y,
        spritebox->widget.rect.width,
        spritebox->widget.rect.height
    };
    SDL_RenderTexture(renderer, hmget(texture_map, spritebox->tid), &src_rect, &dst_rect);
}

void render_button(SDL_Renderer *renderer, texture_hash *texture_map, Button *button){
    if (!button->widget.rect.visible) {return;}
    int rel_offset_x, rel_offset_y;
    SDL_FRect dst_rect = {
        button->widget.rect.pos.x,
        button->widget.rect.pos.y,
        button->widget.rect.width,
        button->widget.rect.height
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
        (rel_offset_x)*(button->widget.rect.width + 2),
        (rel_offset_y)*(button->widget.rect.height + 2),
        button->widget.rect.width,
        button->widget.rect.height
    };
    SDL_RenderTexture(renderer, hmget(texture_map, button->tid), &src_rect, &dst_rect);
}

void render_label(SDL_Renderer *renderer, texture_hash* texture_map, font_hash* font_map, Label *label){
    if (!label->widget.rect.visible) {return;}
    SDL_FRect dst_rect = {
        label->widget.rect.pos.x,
        label->widget.rect.pos.y,
        label->widget.rect.width,
        label->widget.rect.height
    };
    if (label->_retex){
        rerender_font_texture(renderer, texture_map, font_map, label->tid, label->fid, label->txt);
        label->_retex = false;
    }
    SDL_RenderTexture(renderer, hmget(texture_map, label->tid), NULL, &dst_rect);
}

void render_widgets(SDL_Renderer *renderer, texture_hash* texture_map, font_hash* font_map, Container *root){
    Widget** children = container_get_children(root);
    for (int i = 0; i < arrlen(children); i++){
        if (children[i] == NULL) {return;}
        switch(children[i]->wtype){
            case WIDGET_CONTAINER:
                render_widgets(renderer, texture_map, font_map, (Container *)children[i]);
                break;
            case WIDGET_PICBOX:
                render_picturebox(renderer, texture_map, (PictureBox *)children[i]);
                break;
            case WIDGET_SPRITEBOX:
                render_spritebox(renderer, texture_map, (SpriteBox *)children[i]);
                break;
            case WIDGET_BUTTON:
                render_button(renderer, texture_map, (Button *)children[i]);
                break;
            case WIDGET_LABEL:
                render_label(renderer, texture_map, font_map, (Label *)children[i]);
                break;
            default:
                break;
        }
    }
}

void render_card(SDL_Renderer *renderer, texture_hash *texture_map, Card card){
    if (!card.rect.visible) {return;}
    SDL_FRect dst_rect = {
        card.rect.pos.x,
        card.rect.pos.y,
        card.rect.width,
        card.rect.height
    };
    int rel_offset_x = 0, rel_offset_y = 0;
    if (card.face_down){
        rel_offset_x = 0;
        rel_offset_y = 4;
    }
    else {
        for (int i=0; i<13; i++){
            if (card.rank == RANKS[i]){
                rel_offset_x = i;
                break;
            }
        }
        for (int j=0; j<4; j++){
            if (card.suit == SUITS[j]){
                rel_offset_y = j;
                break;
            }
        }
    }
    SDL_FRect src_rect = {
        rel_offset_x*(card.rect.width + 2),
        rel_offset_y*(card.rect.height + 2),
        card.rect.width,
        card.rect.height
    };
    SDL_RenderTexture(renderer, hmget(texture_map, TEXTURE_ID_CARD_SPRITESHEET), &src_rect, &dst_rect);
}

void render_cards(SDL_Renderer *renderer, texture_hash* texture_map, GameContext *game_ctx){
    int deck_card_count = *game_ctx->deck_top_index_ptr + 1;
    for (int i = 0; i < deck_card_count; i++){
        render_card(renderer, texture_map, game_ctx->deck[i]);
    }
    /*
    i.e render cards outside of deck NOT being animated/which are visually in-hand in order of when
    they were drawn.
    */
    for (int i = deck_card_count; i < 52; i++){
        if (
            game_ctx->deck[i].rect.pos.x == DECK_ORIGIN_X - (51 - i) 
            && game_ctx->deck[i].rect.pos.y == DECK_ORIGIN_Y + (51 - i)
        ){
            render_card(renderer, texture_map, game_ctx->deck[i]);
        }
    }
    /*
    i.e render cards outside of deck which are visually in-hand/being animated in reverse order
    of when they were drawn.
    */
    for (int i = 51; i >= deck_card_count; i--){
        if (
            game_ctx->deck[i].rect.pos.x != DECK_ORIGIN_X - (51 - i) 
            && game_ctx->deck[i].rect.pos.y != DECK_ORIGIN_Y + (51 - i)
        ){
            render_card(renderer, texture_map, game_ctx->deck[i]);
        }
    }
}

void render(AppState *as){
    SDL_RenderTexture(
        as->renderer, 
        hmget(as->texture_map, TEXTURE_ID_BACKGROUND), 
        NULL, 
        NULL
    );
    render_cards(as->renderer, as->texture_map, as->gctx);
    render_widgets(as->renderer, as->texture_map, as->font_map, as->ui_root);
    SDL_RenderPresent(as->renderer);
}

/*

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