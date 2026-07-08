#ifndef GAME_OBJECTS_H
#define GAME_OBJECTS_H

#include <SDL3/SDL.h>
#include <stdbool.h>
#include "../include/game_object.h"
#include "../include/game.h"

#define MAXIMUM_ALIVE_TEXTURED_GAME_OBJS 8

typedef struct TexturedGameObject {
    GameObject obj;
    SDL_Texture *texture;
} TexturedGameObject;

typedef struct Chip {
    TexturedGameObject tex_obj;
    CHIP_VALUE val;
} Chip;

TexturedGameObject* create_textured_game_object(int x, int y, int width, int height, bool visible, SDL_Texture *texture);
void destroy_textured_game_object(TexturedGameObject *ptextured_obj);
Chip* create_chip(int x, int y, int width, int height, bool visible, SDL_Texture *texture, CHIP_VALUE val);
void destroy_chip(Chip *pchip);
void destroy_all_textured_game_objects(void);

extern TexturedGameObject* alive_textured_game_objects[MAXIMUM_ALIVE_TEXTURED_GAME_OBJS];

#endif