#include <SDL3/SDL.h>
#include <SDL3/SDL_events.h>
#include <stdbool.h>
#include "../include/stb_ds.h"
#include "../include/widget.h"
#include "../include/button.h"
#include "../include/input.h"

static Widget** subscribers = NULL;

void input_subscriber_add(Widget *widget){
    arrput(subscribers, widget);
}

void input_subscriber_remove(Widget *widget){
    for (int i = 0; i < arrlen(subscribers); i++){
        if (subscribers[i] == widget){
            arrdel(subscribers, i);
        }
    }
}

void input_subscribers_notify_all(SDL_Event event){
    App_SDL_Event ase = {.type=APP_EVENT_TYPE_SDL, .sdl=event};
    App_Event ae = {.sdl=ase};
    for (int i = 0; i < arrlen(subscribers); i++){
        subscribers[i]->update_func(subscribers[i], ae);
    }
}

bool handle_quit(SDL_Event event){
    switch(event.type){
        case SDL_EVENT_QUIT:
            return true;
            break;
        case SDL_EVENT_KEY_DOWN:
            if (event.key.key == SDLK_ESCAPE){
                return true;
            }
            break;
    }
    return false;
}

void button_handle_mouse_events(Button *button, SDL_Event event){
    float x, y;
    SDL_MouseButtonFlags mflags = SDL_GetMouseState(&x, &y);
    if(x > button->widget.pos.x 
        && x < (button->widget.pos.x + button->widget.width)
        && y > button->widget.pos.y 
        && y < (button->widget.pos.y + button->widget.height)
    ){
        if (button_get_state(button)!=BUTTON_STATE_DISABLED){
            switch (event.type) {
                case SDL_EVENT_MOUSE_BUTTON_DOWN:
                    if (event.button.button == SDL_BUTTON_LEFT){
                        button_set_state(button, BUTTON_STATE_PRESSED);
                    }
                    break;
                case SDL_EVENT_MOUSE_BUTTON_UP:
                    if (event.button.button == SDL_BUTTON_LEFT){
                        button_set_state(button, BUTTON_STATE_RELEASED);
                    }
                    break;
                default:
                    if (button_get_state(button)!=BUTTON_STATE_PRESSED 
                        && button_get_state(button)!=BUTTON_STATE_RELEASED
                    ){
                        button_set_state(button, BUTTON_STATE_HOVERED);
                    }
                    break;
            }
        }  
    }
    else{
        if (button_get_state(button)!=BUTTON_STATE_DISABLED){
            button_set_state(button, BUTTON_STATE_IDLE);
        }
    }
}

bool handle_input(void){
    bool should_quit = false;
    SDL_Event event;
    while (SDL_PollEvent(&event)){
        should_quit = handle_quit(event);
        input_subscribers_notify_all(event);
    }
    return should_quit;
}
