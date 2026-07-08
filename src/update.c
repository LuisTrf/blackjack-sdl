#include <SDL3/SDL.h>
#include "../include/game_constants.h"
#include "../include/buttons.h"
#include "../include/update.h"

static int previous_frame_time = 0;
static float delta_time = 0;

void update_delta_time(void){
    float time_to_wait = TARGET_FRAME_TIME - (SDL_GetTicks() - previous_frame_time);
    if (time_to_wait > 0 && time_to_wait < TARGET_FRAME_TIME){
        SDL_Delay(time_to_wait);
    }
    delta_time = (SDL_GetTicks() - previous_frame_time)/1000.f;
    previous_frame_time = SDL_GetTicks();
}

float get_delta_time(void){
    return delta_time;
}

void do_button_actions(void){
    for (int i=0; i<NUMBER_OF_BUTTONS; i++){
        if (get_state(buttons[i])==RELEASED){
            buttons[i]->action(buttons[i]);
        }
    }
}

void update_action_button_positions_from_visibility(void){
    int remaining_visible_action_buttons = get_visible_action_buttons();
    for (int i=0; i<NUMBER_OF_ACTION_BUTTONS; i++){
        if (action_buttons[i]->obj.visible){
            action_buttons[i]->obj.x = 
                BUTTON_X_ORIGIN
                + 0.5f * (get_visible_action_buttons()-1) * BUTTON_WIDTH
                - 0.5f * (remaining_visible_action_buttons-1) * (BUTTON_STEP_X + BUTTON_WIDTH);
            remaining_visible_action_buttons--;
        }
        else {
            action_buttons[i]->obj.x = BUTTON_X_ORIGIN;
        }
    }
}

void update(void){
    update_delta_time();
    do_button_actions();
    update_action_button_positions_from_visibility();
}