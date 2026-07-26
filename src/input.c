#include <SDL3/SDL.h>
#include <SDL3/SDL_events.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../include/stb_ds.h"
#include "../include/widget.h"
#include "../include/button.h"
#include "../include/input.h"

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

InputContext* input_context_create(void){
    InputContext ic = {NULL};
    InputContext *p_ic = malloc(sizeof(InputContext));
    if (p_ic == NULL){
        abort();
    }
    *p_ic = ic;
    return p_ic;
}

void input_context_destroy(InputContext *p_ic){
    arrfree(p_ic->input_widget_listeners);
    p_ic->input_widget_listeners = NULL;
    free(p_ic);
    p_ic = NULL;
}

void ic_widget_listener_register(InputContext *p_ic, Widget *widget){
    arrput(p_ic->input_widget_listeners, widget);
}

void ic_widget_listener_remove(InputContext *p_ic, Widget *widget){
    for (int i = 0; i < arrlen(p_ic->input_widget_listeners); i++){
        if (p_ic->input_widget_listeners[i] == widget){
            arrdel(p_ic->input_widget_listeners, i);
        }
    }
}

void ic_widget_listeners_notify_all(EventQueue *event_queue, Widget** widget_listeners, SDL_Event event){
    for (int i = 0; i < arrlen(widget_listeners); i++){
        Event e = widget_listeners[i]->input_func(widget_listeners[i], event);
        if (!event_is_null(e)){
            enqueue_event(event_queue, e);
        }
    }
}

Event input_handle_button_mouse_events(Widget *widget, SDL_Event event){
    Button *button = (Button *)widget;
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
                        return button->release_event;
                        /*
                        button->callback(button);
                        button_notify_all(button, button->release_event);
                        */
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

bool input_handle(InputContext *p_ic, EventQueue *event_queue){
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
