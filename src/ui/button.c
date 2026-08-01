#include <SDL3/SDL.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../../include/ui/button_constants.h"
#include "../../include/event/event.h"
#include "../../include/ui/button.h"
#include "../../include/stb_ds.h"

/*
Button deal_button = {{BUTTON_X_ORIGIN, BUTTON_Y_ORIGIN, BUTTON_WIDTH, BUTTON_HEIGHT, true},
    IDLE, _NONE_BUTTON_STATE, NULL, on_deal_released};
Button bet_button = {{BUTTON_X_ORIGIN, BUTTON_Y_ORIGIN, BUTTON_WIDTH, BUTTON_HEIGHT, true},
    IDLE, _NONE_BUTTON_STATE, NULL, on_bet_released};
Button hit_button = {{BUTTON_X_ORIGIN, BUTTON_Y_ORIGIN, BUTTON_WIDTH, BUTTON_HEIGHT, false},
    DISABLED, _NONE_BUTTON_STATE, NULL, on_hit_released};
Button stand_button = {{BUTTON_X_ORIGIN, BUTTON_Y_ORIGIN, BUTTON_WIDTH, BUTTON_HEIGHT, false},
    DISABLED, _NONE_BUTTON_STATE, NULL, on_stand_released};
Button insurance_button = {{BUTTON_X_ORIGIN, BUTTON_Y_ORIGIN, BUTTON_WIDTH, BUTTON_HEIGHT, false},
    DISABLED, _NONE_BUTTON_STATE, NULL, on_insurance_released};
Button double_down_button = {{BUTTON_X_ORIGIN, BUTTON_Y_ORIGIN, BUTTON_WIDTH, BUTTON_HEIGHT, false},
    DISABLED, _NONE_BUTTON_STATE, NULL, on_double_down_released};
Button stack_button = {{STACK_BUTTON_X_ORIGIN, STACK_BUTTON_Y_ORIGIN, CHIP_BUTTON_WIDTH, CHIP_BUTTON_HEIGHT, false},
    DISABLED, _NONE_BUTTON_STATE, NULL, on_stack_released};
Button white1_button = {{CHIP_BUTTON_X(0), CHIP_BUTTON_Y(0), CHIP_BUTTON_WIDTH, CHIP_BUTTON_HEIGHT, false},
    DISABLED, _NONE_BUTTON_STATE, NULL, on_valued_released};
Button red5_button = {{CHIP_BUTTON_X(1), CHIP_BUTTON_Y(1), CHIP_BUTTON_WIDTH, CHIP_BUTTON_HEIGHT, false},
    DISABLED, _NONE_BUTTON_STATE, NULL, on_valued_released};
Button blue10_button = {{{CHIP_BUTTON_X(2), CHIP_BUTTON_Y(2), CHIP_BUTTON_WIDTH, CHIP_BUTTON_HEIGHT, false},
    DISABLED, _NONE_BUTTON_STATE, NULL, on_valued_released}, TEN};
ValuedButton green25_button = {{{CHIP_BUTTON_X(3), CHIP_BUTTON_Y(3), CHIP_BUTTON_WIDTH, CHIP_BUTTON_HEIGHT, false},
    DISABLED, _NONE_BUTTON_STATE, NULL, on_valued_released}, TWENTY_FIVE};
ValuedButton black100_button = {{{CHIP_BUTTON_X(4), CHIP_BUTTON_Y(4), CHIP_BUTTON_WIDTH, CHIP_BUTTON_HEIGHT, false},
    DISABLED, _NONE_BUTTON_STATE, NULL, on_valued_released}, HUNDRED};
ValuedButton purple500_button = {{{CHIP_BUTTON_X(5), CHIP_BUTTON_Y(5), CHIP_BUTTON_WIDTH, CHIP_BUTTON_HEIGHT, false},
    DISABLED, _NONE_BUTTON_STATE, NULL, on_valued_released}, FIVE_HUNDRED};
ValuedButton yellow1k_button = {{{CHIP_BUTTON_X(6), CHIP_BUTTON_Y(6), CHIP_BUTTON_WIDTH, CHIP_BUTTON_HEIGHT, false},
    DISABLED, _NONE_BUTTON_STATE, NULL, on_valued_released}, ONE_K};
ValuedButton orange5k_button = {{{CHIP_BUTTON_X(7), CHIP_BUTTON_Y(7), CHIP_BUTTON_WIDTH, CHIP_BUTTON_HEIGHT, false},
    DISABLED, _NONE_BUTTON_STATE, NULL, on_valued_released}, FIVE_K};
Button redblue25k_button = {{{CHIP_BUTTON_X(8), CHIP_BUTTON_Y(8), CHIP_BUTTON_WIDTH, CHIP_BUTTON_HEIGHT, false},
    DISABLED, _NONE_BUTTON_STATE, NULL, on_valued_released}, TWENTY_FIVE_K};
Button gold100k_button = {{{CHIP_BUTTON_X(9), CHIP_BUTTON_Y(9), CHIP_BUTTON_WIDTH, CHIP_BUTTON_HEIGHT, false},
    DISABLED, _NONE_BUTTON_STATE, NULL, on_valued_released}, HUNDRED_K};
*/

Button* button_create(
    float x, float y, 
    int width, int height, 
    bool visible, 
    EventType release_eventtype,
    BUTTON_STATE button_state_initial, 
    SDL_Texture* spritesheet, 
    Event (*notify_func)(Widget *self, Event event),
    Event (*input_func)(Widget *self, SDL_Event event)
)
{
    Button button = {
        .widget={
            .rect = {
                .pos={.x=x, .y=y}, 
                .width=width, 
                .height=height, 
                .visible=visible, 
            },
            .wtype=WIDGET_BUTTON,
            .notify_func=notify_func,
            .input_func=input_func
        }, 
        .release_event=common_event_create(release_eventtype), 
        ._state=button_state_initial, 
        ._prev_state=_BUTTON_STATE_NONE, 
        .spritesheet=spritesheet, 
    };
    Button *p_button = malloc(sizeof(Button));
    if (p_button == NULL){
        abort();
    }
    *p_button = button;
    return p_button;
}

ButtonContext* button_context_create(void){
    ButtonContext button_ctx = {NULL};
    ButtonContext *p_button_ctx = malloc(sizeof(ButtonContext));
    if (p_button_ctx == NULL){
        abort();
    }
    *p_button_ctx = button_ctx;
    return p_button_ctx;
}

void button_destroy(Button *p_button){
    free(p_button);
    p_button = NULL;
}

void button_context_destroy(ButtonContext *button_ctx){
    arrfree(button_ctx->moveb_refs);
    button_ctx->moveb_refs = NULL;
    free(button_ctx);
}

Event button_get_release_event(Button *button){
    return button->release_event;
}

BUTTON_STATE button_get_state(Button *button){
    return button->_state;
}

void button_set_state(Button *button, BUTTON_STATE state){
    button->_prev_state = button->_state;
    button->_state = state;
}

BUTTON_STATE button_get_prev_state(Button *button){
    return button->_prev_state;
}

void button_restore_prev_state(Button *button){
    BUTTON_STATE temp = button->_state;
    button->_state = button->_prev_state;
    button->_prev_state = temp;
}

void button_ctx_register_move_button(ButtonContext *button_ctx, Button *button){
    arrput(button_ctx->moveb_refs, button);
}

int button_ctx_get_visible_move_buttons(ButtonContext *button_ctx){
    int visible_move_button_count = 0;
    for (int i = 0; i < arrlen(button_ctx->moveb_refs); i++){
        if (button_ctx->moveb_refs[i]->widget.rect.visible){
            visible_move_button_count++;
        }
    }
    return visible_move_button_count;
}

void button_ctx_reposition_visible_move_buttons(ButtonContext *button_ctx){
    int visible_move_button_count = button_ctx_get_visible_move_buttons(button_ctx);
    int remaining_visible_move_button_count = visible_move_button_count;
    for (int i = 0; i < arrlen(button_ctx->moveb_refs); i++){
        if (button_ctx->moveb_refs[i]->widget.rect.visible){
            button_ctx->moveb_refs[i]->widget.rect.pos.x = 
                MOVE_BUTTON_ORIGIN_X
                + 0.5f * (visible_move_button_count - 1) * MOVE_BUTTON_WIDTH
                - 0.5f * (remaining_visible_move_button_count - 1) * (MOVE_BUTTON_STEP_X + MOVE_BUTTON_WIDTH);
            remaining_visible_move_button_count--;
        }
        else {
            button_ctx->moveb_refs[i]->widget.rect.pos.x = MOVE_BUTTON_ORIGIN_X;
        }
    }
}

Event button_notify_deal(Widget *self, Event event){
    switch(event.type){
        case BUTTON_EVENT_RELEASE_DEAL:
            button_set_state((Button *)self, BUTTON_STATE_DISABLED);
            self->rect.visible = false;
            break;
        default:
            break;
    }
    return NULL_EVENT;
}

Event button_notify_hit(Widget *self, Event event){
    switch(event.type){
        case BUTTON_EVENT_RELEASE_DEAL:
            button_set_state((Button *)self, BUTTON_STATE_IDLE);
            self->rect.visible = true;
            break;
        default:
            break;
    }
    return NULL_EVENT;
}

Event button_notify_stand(Widget *self, Event event){
    switch(event.type){
        case BUTTON_EVENT_RELEASE_DEAL:
            button_set_state((Button *)self, BUTTON_STATE_IDLE);
            self->rect.visible = true;
            break;
        default:
            break;
    }
    return NULL_EVENT;
}