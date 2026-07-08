#include <SDL3/SDL.h>
#include <SDL3/SDL_events.h>
#include <stdbool.h>
#include "../include/buttons.h"
#include "../include/input.h"

/*
returns false on quit; true otherwise
*/
bool handle_quit(SDL_Event *event){
    switch(event->type){
        case SDL_EVENT_QUIT:
            return false;
            break;
        case SDL_EVENT_KEY_DOWN:
            if (event->key.key == SDLK_ESCAPE){
                return false;
            }
            break;
    }
    return true;
}

void handle_button_mouse_events(Button *button, SDL_Event *event){
    float x, y;
    SDL_MouseButtonFlags mflags = SDL_GetMouseState(&x, &y);
    if(x > button->obj.x 
        && x < (button->obj.x + button->obj.width)
        && y > button->obj.y 
        && y < (button->obj.y + button->obj.height)
    ){
        if (get_state(button)!=DISABLED){
            switch (event->type) {
                case SDL_EVENT_MOUSE_BUTTON_DOWN:
                    if (event->button.button == SDL_BUTTON_LEFT){
                        set_state(button, PRESSED);
                    }
                    break;
                case SDL_EVENT_MOUSE_BUTTON_UP:
                    if (event->button.button == SDL_BUTTON_LEFT){
                        set_state(button, RELEASED);
                    }
                    break;
                default:
                    if (get_state(button)!=PRESSED && get_state(button)!=RELEASED){
                        set_state(button, HOVERED);
                    }
                    break;
            }
        }  
    }
    else{
        if (get_state(button)!=DISABLED){
            set_state(button, IDLE);
        }
    }
}


void handle_buttons(SDL_Event *event){
    for (int i=0; i<NUMBER_OF_BUTTONS; i++){
        handle_button_mouse_events(buttons[i], event);
    }
}

bool handle_input(void){
    bool running = true;
    SDL_Event event;
    while (SDL_PollEvent(&event)){
        running=handle_quit(&event);
        handle_buttons(&event);
    }
    return running;
}
