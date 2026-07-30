#include "../../include/ui/widget_internal.h"

Event widget_notify(Widget *widget, Event event){
    return widget->notify_func(widget, event);
}

Event widget_input(Widget *widget, SDL_Event event){
    return widget->input_func(widget, event);
}