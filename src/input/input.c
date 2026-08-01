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

InputContext* input_context_create(void){
    InputContext input_ctx = {NULL};
    InputContext *p_input_ctx = malloc(sizeof(InputContext));
    if (p_input_ctx == NULL){
        abort();
    }
    *p_input_ctx = input_ctx;
    return p_input_ctx;
}

void input_context_destroy(InputContext *input_ctx){
    arrfree(input_ctx->input_widget_listeners);
    input_ctx->input_widget_listeners = NULL;
    free(input_ctx);
}

void input_ctx_widget_listener_register(InputContext *input_ctx, Widget *widget){
    arrput(input_ctx->input_widget_listeners, widget);
}

void input_ctx_widget_listener_remove(InputContext *input_ctx, Widget *widget){
    for (int i = 0; i < arrlen(input_ctx->input_widget_listeners); i++){
        if (input_ctx->input_widget_listeners[i] == widget){
            arrdel(input_ctx->input_widget_listeners, i);
        }
    }
}

void input_ctx_widget_listeners_notify_all(EventContext *event_ctx, InputContext* input_ctx, SDL_Event sdl_event){;
    for (int i = 0; i < arrlen(input_ctx->input_widget_listeners); i++){
        Event e = widget_input(input_ctx->input_widget_listeners[i], sdl_event);
        if (!event_is_null(e)){
            event_enqueue(event_ctx->queue, e);
        }
    }
}

Event input_handle_button_mouse_events(Widget *widget, SDL_Event sdl_event){
    Button *button = (Button *)widget;
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
        input_ctx_widget_listeners_notify_all(
            as->event_ctx, 
            as->input_ctx,
            sdl_event
        );
    }
    return should_quit;
}
