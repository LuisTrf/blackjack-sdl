#ifndef BUTTON_EVENTS_H
#define BUTTON_EVENTS_H

#include "../include/buttons.h"

void on_deal_released(Button *self);
void on_bet_released(Button *self);
void on_hit_released(Button *self);
void on_stand_released(Button *self);
void on_stack_released(Button *self);
void on_valued_released(Button *self);
void on_insurance_released(Button *self);
void on_double_down_released(Button *self);

#endif