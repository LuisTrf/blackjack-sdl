#pragma once

#include "../rect.h"

typedef enum EventType {
    EVENT_NONE,
    EVENT_ANIM_QUEUE_BLOCKING,
    EVENT_ANIM_QUEUE_NONBLOCKING,
    BUTTON_EVENT_RELEASE_DEAL,
    BUTTON_EVENT_RELEASE_HIT,
    BUTTON_EVENT_RELEASE_STAND,
    ANIMATION_EVENT_ANIMATION_CARD_DRAW_COMPLETED,
    EVENT_NEW_GAME
} EventType;

typedef struct CommonEvent {
    EventType type;
} CommonEvent;

typedef struct AnimationEvent {
    EventType type;
    Rect *target;
} AnimationEvent;

typedef union Event {
    EventType type;
    CommonEvent common;
    AnimationEvent anim;
} Event;

static const Event NULL_EVENT = {.type = EVENT_NONE};