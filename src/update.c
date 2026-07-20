#include <SDL3/SDL.h>
#include <stdlib.h>
#include "../include/constants.h"
#include "../include/button.h"
#include "../include/update.h"

UpdateContext* update_context_initialize(void){
    UpdateContext uc = {SDL_GetTicks(), 0.f};
    UpdateContext *p_uc = malloc(sizeof(UpdateContext));
    if (p_uc == NULL){
        abort();
    }
    *p_uc = uc;
    return p_uc;
}

void update_context_teardown(UpdateContext *p_uc){
    free(p_uc);
    p_uc = NULL;
}

void update_delta_time(UpdateContext *p_uc){
    float time_to_wait = TARGET_FRAME_TIME - (SDL_GetTicks() - p_uc->previous_frame_time);
    if (time_to_wait > 0 && time_to_wait < TARGET_FRAME_TIME){
        SDL_Delay(time_to_wait);
    }
    p_uc->delta_time = (SDL_GetTicks() - p_uc->previous_frame_time)/1000.f;
    p_uc->previous_frame_time = SDL_GetTicks();
}

void update(UpdateContext *p_uc, ButtonContext *p_bc){
    update_delta_time(p_uc);
    button_context_update_action_button_positions_from_visibilities(p_bc);
}