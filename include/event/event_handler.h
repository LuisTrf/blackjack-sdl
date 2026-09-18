#pragma once

#include "../main.h"

void event_handler_handle_deal_input_event(AppState *as);
void event_handler_handle_bet_input_event(AppState *as);
void event_handler_handle_hit_input_event(AppState *as);
void event_handler_handle_stand_input_event(AppState *as);
void event_handler_handle_split_input_event(AppState *as);
void event_handler_handle_double_down_input_event(AppState *as);
void event_handler_handle_insure_input_event(AppState *as);
void event_handler_handle_stack_input_event(AppState *as);
void event_handler_handle_cheque_input_event(AppState *as, CHEQUE_VALUE val);

void event_handler_handle_card_draw_completed_animation_event(AppState *as, Rect *target);
void event_handler_handle_cheque_completed_animation_event(AppState *as);