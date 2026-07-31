#pragma once

typedef enum EventType {
    EVENT_NONE,
    EVENT_ANIM_QUEUE_FINISHED,
    EVENT_CARD_DRAWN,
    EVENT_NEW_GAME,
    BUTTON_EVENT_RELEASE_DEAL,
    BUTTON_EVENT_RELEASE_HIT,
    BUTTON_EVENT_RELEASE_STAND,
} EventType;

typedef struct CommonEvent {
    EventType type;
} CommonEvent;

typedef union Event {
    EventType type;
    CommonEvent common;
} Event;

static const Event NULL_EVENT = {.type = EVENT_NONE};