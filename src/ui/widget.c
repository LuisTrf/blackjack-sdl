#include "../../include/ui/widget_internal.h"

Event widget_notify(Widget *widget, Event event){
    return widget->notify_func(widget, event);
}

Event widget_input(Widget *widget, SDL_Event event){
    return widget->input_func(widget, event);
}

WIDGET_TYPE widget_get_type(Widget* widget){return widget->wtype;}
vec2* widget_get_pos(Widget* widget){return &widget->pos;}
float widget_get_x(Widget *widget){return widget->pos.x;}
float widget_get_y(Widget *widget){return widget->pos.y;}
int widget_get_width(Widget *widget){return widget->width;}
int widget_get_height(Widget *widget){return widget->height;}
bool widget_is_visible(Widget *widget){return widget->visible;}
void widget_set_visibility(Widget *widget, bool visibility){widget->visible = visibility;}