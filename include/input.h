#pragma once

#include <SDL3/SDL.h>
#include <stdbool.h>
#include "widget.h"
#include "button.h"

void input_subscriber_add(Widget *widget);
void input_subscriber_remove(Widget *widget);
bool handle_input(void);
void button_handle_mouse_events(Button *button, SDL_Event event);