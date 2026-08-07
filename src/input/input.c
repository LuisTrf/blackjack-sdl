#include <SDL3/SDL.h>
#include <SDL3/SDL_events.h>
#include <stdbool.h>

#include "../../include/stb_ds.h"
#include "../../include/event/event.h"
#include "../../include/ui/button.h"
#include "../../include/input/input.h"
#include "../../include/main.h"

bool handle_quit(SDL_Event sdl_event){
    switch(sdl_event.type){
        case SDL_EVENT_QUIT:
            return true;
            break;
        case SDL_EVENT_KEY_DOWN:
            if (sdl_event.key.key == SDLK_ESCAPE){
                return true;
            }
            break;
    }
    return false;
}

InputListener input_listener_create(void * self, Event (*input_func)(void *self, SDL_Event sdl_event)){
    InputListener input_listener = {self, input_func};
    return input_listener;
}

void input_listener_register(InputListener **input_listeners, InputListener input_listener){
    arrput(*input_listeners, input_listener);
}

void input_listener_remove(InputListener **input_listeners, InputListener input_listener){
    for (int i = 0; i < arrlen(*input_listeners); i++){
        if ((*input_listeners)[i].self == input_listener.self){
            arrdel(*input_listeners, i);
        }
    }
}

void input_listeners_notify_all(EventQueue *queue, InputListener **input_listeners, SDL_Event sdl_event){;
    for (int i = 0; i < arrlen(*input_listeners); i++){
        Event event = (*input_listeners)[i].input_func((*input_listeners)[i].self, sdl_event);
        if (!event_is_null(event)){
            event_enqueue(queue, event);
        }
    }
}

Event input_handle_button_mouse_events(void *self, SDL_Event sdl_event){
    Button *button = (Button *)self;
    float x, y;
    float bx = button->widget.rect.pos.x; 
    float by = button->widget.rect.pos.y;
    int bwidth = button->widget.rect.width; 
    int bheight = button->widget.rect.height;
    SDL_MouseButtonFlags mflags = SDL_GetMouseState(&x, &y);
    if(x > bx
        && x < (bx + bwidth)
        && y > by
        && y < (by + bheight)
    ){
        if (button_get_state(button) != BUTTON_STATE_DISABLED){
            switch (sdl_event.type) {
                case SDL_EVENT_MOUSE_BUTTON_DOWN:
                    if (sdl_event.button.button == SDL_BUTTON_LEFT){
                        button_set_state(button, BUTTON_STATE_PRESSED);
                    }
                    break;
                case SDL_EVENT_MOUSE_BUTTON_UP:
                    if (
                        button_get_state(button) == BUTTON_STATE_PRESSED 
                        && sdl_event.button.button == SDL_BUTTON_LEFT
                    )
                    {
                        button_restore_prev_state(button);
                        return button->release_event;
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
        if (button_get_state(button) != BUTTON_STATE_DISABLED){
            button_set_state(button, BUTTON_STATE_IDLE);
        }
    }
    return NULL_EVENT;
}

bool input_handle(AppState *as){
    bool should_quit = false;
    SDL_Event sdl_event;
    while (SDL_PollEvent(&sdl_event)){
        should_quit = handle_quit(sdl_event);
        input_listeners_notify_all(
            as->event_queue, 
            as->p_input_listeners,
            sdl_event
        );
    }
    return should_quit;
}
