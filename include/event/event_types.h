#pragma once

#include "../rect.h"
#include "../render/render_types.h"

typedef enum EventType {
    EVENT_NONE,
    INPUT_EVENT_BUTTON_RELEASE_DEAL,
    INPUT_EVENT_BUTTON_RELEASE_HIT,
    INPUT_EVENT_BUTTON_RELEASE_STAND,
    INPUT_EVENT_BUTTON_RELEASE_BET,
    INPUT_EVENT_BUTTON_RELEASE_STACK,
    INPUT_EVENT_BUTTON_RELEASE_WHITE,
    STATE_EVENT_DEAL,
    STATE_EVENT_HIT,
    STATE_EVENT_STAND,
    STATE_EVENT_BET,
    ANIMATION_EVENT_ANIMATION_CARD_DRAW_COMPLETED,
    ANIMATION_EVENT_ANIMATION_CHEQUE_COMPLETED,
    ANIMATION_EVENT_QUEUE_BLOCKING,
    ANIMATION_EVENT_QUEUE_NONBLOCKING,
} EventType;

typedef struct InputEvent {
    EventType type;
    union {
    } data;
} InputEvent;

typedef struct StateEvent {
    EventType type;
    union {
        struct {int dealer_cards_in_hand, dealer_hand_value, player_cards_in_hand, player_hand_value;} hand;
    } data;
} StateEvent;

typedef struct AnimationEvent {
    EventType type;
    Rect *target;
    union {
    } data;
} AnimationEvent;

typedef union Event {
    EventType type;
    InputEvent input;
    StateEvent state;
    AnimationEvent anim;
} Event;

static const Event NULL_EVENT = {.type = EVENT_NONE};