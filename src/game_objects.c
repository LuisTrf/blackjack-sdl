#include <SDL3/SDL.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include "../include/game_objects.h"

TexturedGameObject* alive_textured_game_objects[MAXIMUM_ALIVE_TEXTURED_GAME_OBJS] = {NULL};

int get_available_textured_object_index(void){
    for (int i=0; i<MAXIMUM_ALIVE_TEXTURED_GAME_OBJS; i++){
        if (alive_textured_game_objects[i]==NULL) {return i;}
    }
    return -1;
}

int get_textured_object_index(TexturedGameObject *ptextured_obj){
    for (int i=0; i<MAXIMUM_ALIVE_TEXTURED_GAME_OBJS; i++){
        if (alive_textured_game_objects[i]==ptextured_obj) {return i;}
    }
    return -1;
}

TexturedGameObject* create_textured_game_object(int x, int y, int width, int height, bool visible, SDL_Texture *texture){
    int i = get_available_textured_object_index();
    if (i==-1) {
        fprintf(stderr, "MAXIMUM NUMBER OF ALIVE TEXTURED GAME OBJECTS!\n"); 
        return NULL;
    }
    TexturedGameObject textured_obj = {{x, y, width, height, visible}, texture};
    TexturedGameObject *ptextured_obj = malloc(sizeof(TexturedGameObject));
    if (ptextured_obj==NULL){
        abort();
    }
    *ptextured_obj=textured_obj;
    alive_textured_game_objects[i]=ptextured_obj;
    return ptextured_obj;
}

void destroy_textured_game_object(TexturedGameObject *ptextured_obj){
    int i = get_textured_object_index(ptextured_obj);
    if (i==-1) {
        fprintf(stderr, "PASSED TEXTURED GAME OBJECT IS ILLEGALLY CREATED/DOESN'T EXIST!\n");
        return;
    }
    alive_textured_game_objects[i]=NULL;
    free(ptextured_obj);
    ptextured_obj=NULL;
}

void destroy_all_textured_game_objects(void){
    for (int i=0; i<MAXIMUM_ALIVE_TEXTURED_GAME_OBJS; i++){
        if (alive_textured_game_objects[i]!=NULL){
            destroy_textured_game_object(alive_textured_game_objects[i]);
        }
    }
}

Chip* create_chip(int x, int y, int width, int height, bool visible, SDL_Texture *texture, CHIP_VALUE val){
    int i = get_available_textured_object_index();
    if (i==-1) {
        fprintf(stderr, "MAXIMUM NUMBER OF ALIVE TEXTURED GAME OBJECTS!\n"); 
        return NULL;
    }
    Chip chip = {{{x, y, width, height, visible}, texture}, val};
    Chip *pchip = malloc(sizeof(Chip));
    if (pchip==NULL){
        abort();
    }
    *pchip=chip;
    alive_textured_game_objects[i]=&(pchip->tex_obj);
    return pchip;
}

void destroy_chip(Chip *pchip){
    int i = get_textured_object_index(&(pchip->tex_obj));
    if (i==-1) {
        fprintf(stderr, "PASSED TEXTURED GAME OBJECT IS ILLEGALLY CREATED/DOESN'T EXIST!\n");
        return;
    }
    alive_textured_game_objects[i]=NULL;
    free(pchip);
    pchip=NULL;
}
