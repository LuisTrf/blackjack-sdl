#include <SDL3/SDL.h>
#include <SDL3/SDL_events.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../../include/stb_ds.h"
#include "../../include/ui/widget.h"
#include "../../include/event/event.h"
#include "../../include/ui/button.h"
#include "../../include/input/input.h"
#include "../../include/main.h"

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

void ic_widget_listeners_notify_all(Event_Context *ec, Input_Context* ic, SDL_Event event){;
    for (int i = 0; i < arrlen(ic->input_widget_listeners); i++){
        Event e = widget_input(ic->input_widget_listeners[i], event);
        if (!event_is_null(e)){
            enqueue_event(ec->queue, e);
        }
    }
}

Event input_handle_button_mouse_events(Widget *widget, SDL_Event event){
    Button *button = (Button *)widget;
    float x, y;
    float bx = button->widget.pos.x; 
    float by = button->widget.pos.y;
    int bwidth = button->widget.width; 
    int bheight = button->widget.height;
    SDL_MouseButtonFlags mflags = SDL_GetMouseState(&x, &y);
    if(x > bx
        && x < (bx + bwidth)
        && y > by
        && y < (by + bheight)
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

bool input_handle(App_State *as){
    bool should_quit = false;
    SDL_Event event;
    while (SDL_PollEvent(&event)){
        should_quit = handle_quit(event);
        ic_widget_listeners_notify_all(
            as->ec, 
            as->ic,
            event
        );
    }
    return should_quit;
}
