#include <SDL3/SDL.h>
#include <SDL3/SDL_events.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../../include/stb_ds.h"
#include "../../include/ui/widget.h"
#include "../../include/event/event.h"
#include "../../include/ui/button.h"
#include "../../include/input/input_internal.h"

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

Input_Context* input_context_create(void){
    Input_Context ic = {NULL};
    Input_Context *p_ic = malloc(sizeof(Input_Context));
    if (p_ic == NULL){
        abort();
    }
    *p_ic = ic;
    return p_ic;
}

void input_context_destroy(Input_Context *p_ic){
    arrfree(p_ic->input_widget_listeners);
    p_ic->input_widget_listeners = NULL;
    free(p_ic);
    p_ic = NULL;
}

void ic_widget_listener_register(Input_Context *p_ic, Widget *widget){
    arrput(p_ic->input_widget_listeners, widget);
}

void ic_widget_listener_remove(Input_Context *p_ic, Widget *widget){
    for (int i = 0; i < arrlen(p_ic->input_widget_listeners); i++){
        if (p_ic->input_widget_listeners[i] == widget){
            arrdel(p_ic->input_widget_listeners, i);
        }
    }
}

void ic_widget_listeners_notify_all(EventQueue *event_queue, Widget** widget_listeners, SDL_Event event){
    for (int i = 0; i < arrlen(widget_listeners); i++){
        Event e = widget_input(widget_listeners[i], event);
        if (!event_is_null(e)){
            enqueue_event(event_queue, e);
        }
    }
}

Event input_handle_button_mouse_events(Widget *widget, SDL_Event event){
    Button *button = (Button *)widget;
    vec2* bpos = button_get_pos(button);
    float x, y;
    SDL_MouseButtonFlags mflags = SDL_GetMouseState(&x, &y);
    if(x > bpos->x 
        && x < (bpos->x + button_get_width(button))
        && y > bpos->y 
        && y < (bpos->y + button_get_height(button))
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
                        return button_get_release_event(button);
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
    SDL_Event event;
    while (SDL_PollEvent(&event)){
        should_quit = handle_quit(event);
        ic_widget_listeners_notify_all(
            event_queue, 
            p_ic->input_widget_listeners,
            event
        );
    }
    return should_quit;
}
