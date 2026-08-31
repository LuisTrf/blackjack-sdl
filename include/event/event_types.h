#pragma once

#include "../rect.h"
#include "../render/render_types.h"
#include "../game/game.h"

typedef enum EventType {
    EVENT_NONE,
    INPUT_EVENT_BUTTON_RELEASE_DEAL,
    INPUT_EVENT_BUTTON_RELEASE_HIT,
    INPUT_EVENT_BUTTON_RELEASE_STAND,
    INPUT_EVENT_BUTTON_RELEASE_BET,
    INPUT_EVENT_BUTTON_RELEASE_SPLIT,
    INPUT_EVENT_BUTTON_RELEASE_STACK,
    INPUT_EVENT_BUTTON_RELEASE_WHITE,
    INPUT_EVENT_BUTTON_RELEASE_RED,
    INPUT_EVENT_BUTTON_RELEASE_BLUE,
    INPUT_EVENT_BUTTON_RELEASE_GREEN,
    INPUT_EVENT_BUTTON_RELEASE_BLACK,
    INPUT_EVENT_BUTTON_RELEASE_PURPLE,
    INPUT_EVENT_BUTTON_RELEASE_YELLOW,
    INPUT_EVENT_BUTTON_RELEASE_ORANGE,
    INPUT_EVENT_BUTTON_RELEASE_REDBLUE,
    INPUT_EVENT_BUTTON_RELEASE_GOLD,
    STATE_EVENT_DEAL,
    STATE_EVENT_HIT,
    STATE_EVENT_STAND,
    STATE_EVENT_BET,
    STATE_EVENT_SPLIT,
    STATE_EVENT_SPLIT_POSSIBLE,
    STATE_EVENT_SPLIT_HIT,
    STATE_EVENT_CHEQUE_PUSH_SENT,
    STATE_EVENT_CHEQUE_PUSH_RECEIVED,
    STATE_EVENT_CHEQUE_POP_SENT,
    STATE_EVENT_CHEQUE_POP_RECEIVED,
    STATE_EVENT_BET_PAYOUT,
    STATE_EVENT_GAME_STATE,
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
        struct {GAME_STATE game_state, prev_game_state;} game_state;
        struct {int player_cards_in_split_hand, player_split_hand_value; float bet;} split_hit;
        struct {int player_cards_in_hand, player_hand_value, player_cards_in_split_hand, player_split_hand_value; float money, bet, split_bet;} split;
        struct {int dealer_cards_in_hand, dealer_hand_value, player_cards_in_hand, player_hand_value, player_cards_in_split_hand, player_split_hand_value;} deal, hit, stand;
        struct {float money;} cheque_push_sent, cheque_pop_received, bet, bet_payout;
        struct {float bet; TEXTURE_ID tid;} cheque_push_received, cheque_pop_sent;
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