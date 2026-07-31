#include "../../include/ui/widget.h"

Event widget_notify(Widget *widget, Event event){
    return widget->notify_func(widget, event);
}

Event widget_input(Widget *widget, SDL_Event sdl_event){
    return widget->input_func(widget, sdl_event);
}