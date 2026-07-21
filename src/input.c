#include <SDL3/SDL.h>
#include <SDL3/SDL_events.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../include/stb_ds.h"
#include "../include/widget.h"
#include "../include/button.h"
#include "../include/input.h"

void input_subscriber_add(InputContext *p_ic, Widget *widget){
    arrput(p_ic->widget_subscribers, widget);
}

void input_subscriber_remove(InputContext *p_ic, Widget *widget){
    for (int i = 0; i < arrlen(p_ic->widget_subscribers); i++){
        if (p_ic->widget_subscribers[i] == widget){
            arrdel(p_ic->widget_subscribers, i);
        }
    }
}

void input_subscribers_notify_all(Widget** widget_subscribers, SDL_Event event){
    EventTyped_SDL_Event ase = {.type=EVENT_TYPE_SDL, .event=event};
    Event ae = {.sdl=ase};
    for (int i = 0; i < arrlen(widget_subscribers); i++){
        widget_subscribers[i]->update_func(widget_subscribers[i], ae);
    }
}

InputContext* input_initialize(void){
    InputContext ic = {NULL};
    InputContext *p_ic = malloc(sizeof(InputContext));
    if (p_ic == NULL){
        abort();
    }
    *p_ic = ic;
    return p_ic;
}

void input_teardown(InputContext *p_ic){
    arrfree(p_ic->widget_subscribers);
    p_ic->widget_subscribers = NULL;
    free(p_ic);
    p_ic = NULL;
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
        if (button_get_state(button) != BUTTON_STATE_DISABLED){
            switch (event.type) {
                case SDL_EVENT_MOUSE_BUTTON_DOWN:
                    if (event.button.button == SDL_BUTTON_LEFT){
                        button_set_state(button, BUTTON_STATE_PRESSED);
                    }
                    break;
                case SDL_EVENT_MOUSE_BUTTON_UP:
                    if (
                        button_get_state(button) == BUTTON_STATE_PRESSED 
                        && event.button.button == SDL_BUTTON_LEFT
                    )
                    {
                        button_restore_prev_state(button);
                        button->callback(button);
                        button_notify_all(button, button->release_event);
                    }
                    break;
                default:
                    if (button_get_state(button)!=BUTTON_STATE_PRESSED){
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

bool handle_input(InputContext *p_ic){
    bool should_quit = false;
    SDL_Event event;
    while (SDL_PollEvent(&event)){
        should_quit = handle_quit(event);
        input_subscribers_notify_all(p_ic->widget_subscribers, event);
    }
    return should_quit;
}
