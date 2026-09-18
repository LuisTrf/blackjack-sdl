#include <SDL3/SDL.h>
#include <SDL3/SDL_stdinc.h>

#include "../../vendored/stb/stb_ds.h"
#include "../../include/constants.h"
#include "../../include/audio_types.h"
#include "../../include/event/event.h"
#include "../../include/update/animation.h"

#include "../../include/event/event_handler.h"

void update_delta_time(Uint64 *previous_frametime, float *delta_time){
    float time_to_wait = TARGET_FRAME_TIME - (SDL_GetTicks() - *previous_frametime);
    if (time_to_wait > 0 && time_to_wait < TARGET_FRAME_TIME){
        SDL_Delay(time_to_wait);
    }
    *delta_time = (SDL_GetTicks() - *previous_frametime)/1000.f;
    *previous_frametime = SDL_GetTicks();
}

void update(AppState *as){
    update_delta_time(&(as->prev_frametime), &(as->delta_time));
    
    while (!event_queue_empty(as->event_queue)){
        Event event = event_dequeue(as->event_queue);
        void *dependencies = NULL;
        switch (event.type){
            case INPUT_EVENT_BUTTON_RELEASE_DEAL:
                event_handler_handle_deal_input_event(as);
                break;
            case INPUT_EVENT_BUTTON_RELEASE_HIT:
                event_handler_handle_hit_input_event(as);
                break;
            case INPUT_EVENT_BUTTON_RELEASE_STAND:
                event_handler_handle_stand_input_event(as);
                break;
            case INPUT_EVENT_BUTTON_RELEASE_BET:
                event_handler_handle_bet_input_event(as);
                break;
            case INPUT_EVENT_BUTTON_RELEASE_SPLIT:
                event_handler_handle_split_input_event(as);
                break;
            case INPUT_EVENT_BUTTON_RELEASE_DOUBLE_DOWN:
                event_handler_handle_double_down_input_event(as);
                break;
            case INPUT_EVENT_BUTTON_RELEASE_INSURANCE: 
                event_handler_handle_insure_input_event(as);
                break;
            case INPUT_EVENT_BUTTON_RELEASE_STACK: {
                event_handler_handle_stack_input_event(as);
                break;
            }
            case INPUT_EVENT_BUTTON_RELEASE_WHITE: {
                event_handler_handle_cheque_input_event(as, CHEQUE_VALUE_ONE);
                break;
            }
            case INPUT_EVENT_BUTTON_RELEASE_RED: {
                event_handler_handle_cheque_input_event(as, CHEQUE_VALUE_FIVE);
                break;
            }
            case INPUT_EVENT_BUTTON_RELEASE_BLUE: {
                event_handler_handle_cheque_input_event(as, CHEQUE_VALUE_TEN);
                break;
            }
            case INPUT_EVENT_BUTTON_RELEASE_GREEN: {
                event_handler_handle_cheque_input_event(as, CHEQUE_VALUE_TWENTY_FIVE);
                break;
            }
            case INPUT_EVENT_BUTTON_RELEASE_BLACK: {
                event_handler_handle_cheque_input_event(as, CHEQUE_VALUE_HUNDRED);
                break;
            }
            case INPUT_EVENT_BUTTON_RELEASE_PURPLE: {
                event_handler_handle_cheque_input_event(as, CHEQUE_VALUE_FIVE_HUNDRED);
                break;
            }
            case INPUT_EVENT_BUTTON_RELEASE_YELLOW: {
                event_handler_handle_cheque_input_event(as, CHEQUE_VALUE_ONE_K);
                break;
            }
            case INPUT_EVENT_BUTTON_RELEASE_ORANGE: {
                event_handler_handle_cheque_input_event(as, CHEQUE_VALUE_FIVE_K);
                break;
            }
            case INPUT_EVENT_BUTTON_RELEASE_REDBLUE: {
                event_handler_handle_cheque_input_event(as, CHEQUE_VALUE_TWENTY_FIVE_K);
                break;
            }
            case INPUT_EVENT_BUTTON_RELEASE_GOLD: {
                event_handler_handle_cheque_input_event(as, CHEQUE_VALUE_HUNDRED_K);
                break;
            }
            case STATE_EVENT_DEAL: 
            case STATE_EVENT_HIT: 
            case STATE_EVENT_STAND:
            case STATE_EVENT_BET:
            case STATE_EVENT_SPLIT:
            case STATE_EVENT_DOUBLE_DOWN: 
            case STATE_EVENT_INSURANCE:
            case STATE_EVENT_BET_PAYOUT:
            case STATE_EVENT_SPLIT_HIT: 
            case STATE_EVENT_CHEQUE_PUSH_SENT: 
            case STATE_EVENT_CHEQUE_POP_SENT: {
                dependencies = (void *)as->font_map;
                break;
            }
            default: {
                break;
            }
        }
        event_listeners_notify_all(&as->event_listeners, event, dependencies);
    }

    animate(as);

    while (!event_queue_empty(as->event_queue)){
        Event event = event_dequeue(as->event_queue);
        void *dependencies = NULL;
        switch(event.type){
            case ANIMATION_EVENT_ANIMATION_CARD_DRAW_BEGINNING:
                MIX_PlayTrack(hmget(as->track_map, TRACK_ID_CARD_DEAL), 0);
                break;
            case ANIMATION_EVENT_ANIMATION_CHEQUE_BEGINNING:
                MIX_PlayTrack(hmget(as->track_map, TRACK_ID_CHEQUE_DEAL), 0);
                break;
            case ANIMATION_EVENT_ANIMATION_CARD_DRAW_COMPLETED:
                event_handler_handle_card_draw_completed_animation_event(as, event.anim.target);
                break;
            case ANIMATION_EVENT_ANIMATION_CHEQUE_COMPLETED: {
                event_handler_handle_cheque_completed_animation_event(as);
                break;
            }
            case STATE_EVENT_CHEQUE_PUSH_RECEIVED:
            case STATE_EVENT_CHEQUE_POP_RECEIVED: {
                dependencies = (void *)as->font_map;
                break;
            }
            default: {
                break;
            }
        }
        event_listeners_notify_all(&as->event_listeners, event, dependencies);
    }
}