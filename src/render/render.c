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
    SDL_Surface* surface = TTF_RenderText_Blended_Wrapped(hmget(font_map, fid), txt, 0, FONT_COLOR, 0);
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
    hmput(texture_map, TEXTURE_ID_RED_BUTTON_SPRITESHEET, render_load_texture_from_png(renderer, "resources/button/red5_spritesheet.png"));
    hmput(texture_map, TEXTURE_ID_BLUE_BUTTON_SPRITESHEET, render_load_texture_from_png(renderer, "resources/button/blue10_spritesheet.png"));
    hmput(texture_map, TEXTURE_ID_GREEN_BUTTON_SPRITESHEET, render_load_texture_from_png(renderer, "resources/button/green25_spritesheet.png"));
    hmput(texture_map, TEXTURE_ID_BLACK_BUTTON_SPRITESHEET, render_load_texture_from_png(renderer, "resources/button/black100_spritesheet.png"));
    hmput(texture_map, TEXTURE_ID_PURPLE_BUTTON_SPRITESHEET, render_load_texture_from_png(renderer, "resources/button/purple500_spritesheet.png"));
    hmput(texture_map, TEXTURE_ID_YELLOW_BUTTON_SPRITESHEET, render_load_texture_from_png(renderer, "resources/button/yellow1k_spritesheet.png"));
    hmput(texture_map, TEXTURE_ID_ORANGE_BUTTON_SPRITESHEET, render_load_texture_from_png(renderer, "resources/button/orange5k_spritesheet.png"));
    hmput(texture_map, TEXTURE_ID_REDBLUE_BUTTON_SPRITESHEET, render_load_texture_from_png(renderer, "resources/button/redblue25k_spritesheet.png"));
    hmput(texture_map, TEXTURE_ID_GOLD_BUTTON_SPRITESHEET, render_load_texture_from_png(renderer, "resources/button/gold100k_spritesheet.png"));
    hmput(texture_map, TEXTURE_ID_LABEL_DEALER_HAND, render_create_empty_font_texture(renderer, font_map, FONT_ID_OPENSANS_32PT));
    hmput(texture_map, TEXTURE_ID_LABEL_PLAYER_HAND, render_create_empty_font_texture(renderer, font_map, FONT_ID_OPENSANS_32PT));
    hmput(texture_map, TEXTURE_ID_LABEL_PLAYER_MONEY, render_create_empty_font_texture(renderer, font_map, FONT_ID_OPENSANS_32PT));
    hmput(texture_map, TEXTURE_ID_LABEL_PLAYER_BET, render_create_empty_font_texture(renderer, font_map, FONT_ID_OPENSANS_32PT));
    hmput(texture_map, TEXTURE_ID_WHITE_CHEQUE, render_load_texture_from_png(renderer, "resources/cheque/white1.png"));
    hmput(texture_map, TEXTURE_ID_RED_CHEQUE, render_load_texture_from_png(renderer, "resources/cheque/red5.png"));
    hmput(texture_map, TEXTURE_ID_BLUE_CHEQUE, render_load_texture_from_png(renderer, "resources/cheque/blue10.png"));
    hmput(texture_map, TEXTURE_ID_GREEN_CHEQUE, render_load_texture_from_png(renderer, "resources/cheque/green25.png"));
    hmput(texture_map, TEXTURE_ID_BLACK_CHEQUE, render_load_texture_from_png(renderer, "resources/cheque/black100.png"));
    hmput(texture_map, TEXTURE_ID_PURPLE_CHEQUE, render_load_texture_from_png(renderer, "resources/cheque/purple500.png"));
    hmput(texture_map, TEXTURE_ID_YELLOW_CHEQUE, render_load_texture_from_png(renderer, "resources/cheque/yellow1k.png"));
    hmput(texture_map, TEXTURE_ID_ORANGE_CHEQUE, render_load_texture_from_png(renderer, "resources/cheque/orange5k.png"));
    hmput(texture_map, TEXTURE_ID_REDBLUE_CHEQUE, render_load_texture_from_png(renderer, "resources/cheque/redblue25k.png"));
    hmput(texture_map, TEXTURE_ID_GOLD_CHEQUE, render_load_texture_from_png(renderer, "resources/cheque/gold100k.png"));
    hmput(texture_map, TEXTURE_ID_ARROW_SPRITESHEET, render_load_texture_from_png(renderer, "resources/arrow_spritesheet.png"));
    hmput(texture_map, TEXTURE_ID_SPLIT_BUTTON_SPRITESHEET, render_load_texture_from_png(renderer, "resources/button/split_spritesheet.png"));
    hmput(texture_map, TEXTURE_ID_LABEL_SPLIT_PLAYER_HAND, render_create_empty_font_texture(renderer, font_map, FONT_ID_OPENSANS_32PT));
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
        32,
        32,
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

void render_cheques(SDL_Renderer *renderer, texture_hash* texture_map, GameContext *game_ctx){
    int head = game_ctx->cheque_ring_buffer->head;
    int count = game_ctx->cheque_ring_buffer->count;
    int size = game_ctx->cheque_ring_buffer->size;
    int idx = head;
    for (int i = 0; i < count; i++){
        idx = (head + i) % size;
        SDL_FRect dst_rect = {
            game_ctx->cheque_ring_buffer->arr[idx].rect.pos.x,
            game_ctx->cheque_ring_buffer->arr[idx].rect.pos.y,
            game_ctx->cheque_ring_buffer->arr[idx].rect.width,
            game_ctx->cheque_ring_buffer->arr[idx].rect.height
        };
        SDL_RenderTexture(renderer, hmget(texture_map, game_ctx->cheque_ring_buffer->arr[idx].tid), NULL, &dst_rect);
    }
}

void render(AppState *as){
    SDL_RenderTexture(
        as->renderer, 
        hmget(as->texture_map, TEXTURE_ID_BACKGROUND), 
        NULL, 
        NULL
    );
    render_cards(as->renderer, as->texture_map, as->game_ctx);
    render_widgets(as->renderer, as->texture_map, as->font_map, as->ui_root);
    render_cheques(as->renderer, as->texture_map, as->game_ctx);
    SDL_RenderPresent(as->renderer);
}