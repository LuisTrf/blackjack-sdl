#include <time.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../../include/game/card_constants.h"
#include "../../include/game/game_constants.h"
#include "../../include/game/game.h"

/*
RULES
Dealer hits maximum 10 times AAAA222233 because stands on 17.
Player hits maximum 5 times because 5-card charlie rule.
*/

const char SUITS[4] = {'C', 'D', 'H', 'S'};
const char RANKS[13] = {'2', '3', '4', '5', '6', '7', '8', '9', 'T', 'J', 'Q', 'K', 'A'};
const int RANK_VALUES[13] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 10, 10, 10, 11};

Player* player_create(void){
    Player player = {
        {NULL}, 
        0, 
        0, 
        0, 
        PLAYER_BEGINNING_MONEY,
        0.f,
        0,
        {_CHIP_VALUE_NONE},
    };
    Player *p_player = malloc(sizeof(Player));
    if (p_player == NULL){
        abort();
    }
    *p_player = player;
    return p_player;
}

void player_destroy(Player *player){
    free(player);
}

Dealer* dealer_create(void){
    Dealer dealer = {
        {NULL},
        0,
        0,
        0
    };
    Dealer *p_dealer = malloc(sizeof(Dealer));
    if (p_dealer == NULL){
        abort();
    }
    *p_dealer = dealer;
    return p_dealer;
}

void dealer_destroy(Dealer *dealer){
    free(dealer);
}

Deck* deck_create(void){
    Card** arr = calloc(52, sizeof(Card));
    if (arr == NULL){
        abort();
    }
    for (int s = 0; s < 4; s++){
        for (int r = 0; r < 13; r++){
            Card card = {
                .obj = {
                    .pos = {
                        .x=DECK_ORIGIN_X - (51 - (s*13 + r)),
                        .y=DECK_ORIGIN_Y + (51 - (s*13 + r)),
                    },
                    CARD_WIDTH,
                    CARD_HEIGHT,
                    true,
                },
                .suit=SUITS[s], 
                .rank=RANKS[r], 
                .rank_value=RANK_VALUES[r], 
                .location=CARD_LOCATION_DECK, 
                .face_down=true
            };
            Card *p = malloc(sizeof(Card));
            if (p==NULL){
                abort();
            }
            *p = card;
            arr[s*13+r] = p;
        }
    }
    Deck deck = {51, arr};
    Deck *p_deck = malloc(sizeof(Deck));
    if (p_deck == NULL){
        abort();
    }
    *p_deck = deck;
    srand(time(NULL));
    return p_deck;
}

void deck_destroy(Deck* deck){
    for (int i=0; i<52; i++){
        free(deck->arr[i]);
        deck->arr[i] = NULL;
    }
    free(deck);
}

int deck_get_card_count(Deck *deck){
    return (deck->top + 1);
}

GameContext* game_context_create(void){
    GameContext game_ctx = {
        GAME_STATE_NEW,
        _GAME_STATE_NONE,
        deck_create(),
        player_create(),
        dealer_create(),
    };
    GameContext *p_game_ctx = malloc(sizeof(GameContext));
    if (p_game_ctx == NULL){
        abort();
    }
    *p_game_ctx = game_ctx;
    return p_game_ctx;
}

void game_context_destroy(GameContext *game_ctx){
    deck_destroy(game_ctx->deck);
    game_ctx->deck = NULL;
    player_destroy(game_ctx->player);
    game_ctx->player = NULL;
    dealer_destroy(game_ctx->dealer);
    game_ctx->dealer = NULL;
    free(game_ctx);
}

GAME_STATE game_context_get_game_state(GameContext *game_ctx){
    return game_ctx->game_state;
}

GAME_STATE game_context_get_prev_game_state(GameContext *game_ctx){
    return game_ctx->prev_game_state;
}

void game_context_set_game_state(GameContext *game_ctx, GAME_STATE state){
    game_ctx->prev_game_state = game_ctx->game_state;
    game_ctx->game_state = state;
}

void deck_shuffle(Deck *deck){
    for (int i=deck->top; i>=1; i--){
        int j = rand()%(i+1);
        Card *temp = deck->arr[j];
        deck->arr[j] = deck->arr[i];
        deck->arr[i] = temp;
        deck->arr[j]->obj.pos.x = DECK_ORIGIN_X - (51 - j);
        deck->arr[j]->obj.pos.y = DECK_ORIGIN_Y + (51 - j);
        deck->arr[i]->obj.pos.x = DECK_ORIGIN_X - (51 - i);
        deck->arr[i]->obj.pos.y = DECK_ORIGIN_Y + (51 - i);
    }
}

bool is_deck_empty(Deck *deck){
    return (deck->top == -1);
}

bool is_deck_full(Deck *deck){
    return (deck->top == 51);
}

void deck_push(Deck *deck, Card *card){
    if (!is_deck_full(deck)){
        deck->arr[deck->top] = card;
        deck->top++;
    }
}

Card* deck_pop(Deck *deck){
    if (!is_deck_empty(deck)){
        Card* c = deck->arr[deck->top];
        deck->arr[deck->top] = NULL;
        deck->top--;
        return c;
    }
    else {
        return NULL;
    }
}

Card* draw_random_card(Deck *deck){
    Card *c;
    do{
        c=deck->arr[rand()%(deck->top+1)];
    } while (c->location!=CARD_LOCATION_DECK);
    return c;
}

void game_reset(GameContext *game_ctx){
    for (;game_ctx->dealer->cards_in_hand > 0; game_ctx->dealer->cards_in_hand--){
        game_ctx->dealer->hand[game_ctx->dealer->cards_in_hand-1]->location = CARD_LOCATION_DECK;
        game_ctx->dealer->hand[game_ctx->dealer->cards_in_hand-1]->face_down = true;
        game_ctx->dealer->hand[game_ctx->dealer->cards_in_hand-1]->obj.pos.x = DECK_ORIGIN_X - (51 - deck_get_card_count(game_ctx->deck));
        game_ctx->dealer->hand[game_ctx->dealer->cards_in_hand-1]->obj.pos.y = DECK_ORIGIN_Y + (51 - deck_get_card_count(game_ctx->deck));
        deck_push(game_ctx->deck, game_ctx->dealer->hand[game_ctx->dealer->cards_in_hand-1]);
        game_ctx->dealer->hand[game_ctx->dealer->cards_in_hand-1] = NULL;
    }
    game_ctx->dealer->hand_value = 0;
    game_ctx->dealer->aces_in_hand_worth_11 = 0;

    for (;game_ctx->player->cards_in_hand > 0; game_ctx->player->cards_in_hand--){
        game_ctx->player->hand[game_ctx->player->cards_in_hand-1]->location = CARD_LOCATION_DECK;
        game_ctx->player->hand[game_ctx->player->cards_in_hand-1]->face_down = true;
        game_ctx->player->hand[game_ctx->player->cards_in_hand-1]->obj.pos.x = DECK_ORIGIN_X - (51 - deck_get_card_count(game_ctx->deck));
        game_ctx->player->hand[game_ctx->player->cards_in_hand-1]->obj.pos.y = DECK_ORIGIN_Y + (51 - deck_get_card_count(game_ctx->deck));
        deck_push(game_ctx->deck, game_ctx->player->hand[game_ctx->player->cards_in_hand-1]);
        game_ctx->player->hand[game_ctx->player->cards_in_hand-1] = NULL;
    }
    game_ctx->player->hand_value = 0;
    game_ctx->player->aces_in_hand_worth_11 = 0;
    game_ctx->player->bet = 0;
    game_ctx->player->betted_chips = 0;
}

bool dealer_bust(Dealer *dealer){
    return dealer->hand_value > 21 ? true : false;
}

bool player_bust(Player *player){
    return player->hand_value > 21 ? true : false;
}

bool dealer_is_blackjack(Dealer *dealer){
    return (dealer->hand_value == 21 && dealer->cards_in_hand == 2) ? true : false;
}

bool player_is_blackjack(Player *player){
    return (player->hand_value == 21 && player->cards_in_hand == 2) ? true : false;
}

bool dealer_can_hit(Dealer *dealer){
    return dealer->cards_in_hand < 10 && dealer->hand_value < 21;
}

bool player_can_hit(Player *player){
    return player->cards_in_hand < 5 && player->hand_value < 21;
}

bool dealer_is_hiding_second_card(Dealer *dealer){
    if (dealer->hand[1] != NULL){
        return dealer->hand[1]->face_down;
    }
    else{
        return true;
    }
}

void dealer_reveal_second_card(Dealer *dealer){
    dealer->hand[1]->face_down=false;
}

Card* dealer_hit(Deck *deck, Dealer *dealer){
    if (dealer_can_hit(dealer)){
        Card *c = deck_pop(deck);
        c->location=CARD_LOCATION_DEALER_HAND;

        dealer->hand[dealer->cards_in_hand] = c;
        dealer->cards_in_hand++;
        dealer->hand_value += c->rank_value;
        
        if (c->rank == 'A'){
            dealer->aces_in_hand_worth_11++;
        }
        if (dealer->hand_value > 21 && dealer->aces_in_hand_worth_11 > 0){
            dealer->hand_value -= 10;
            dealer->aces_in_hand_worth_11--;
        }

        return c;
    }
    else {
        return NULL;
    }
}

Card* player_hit(Deck *deck, Player *player){
    if (player_can_hit(player)){
        Card *c = deck_pop(deck);
        c->location=CARD_LOCATION_PLAYER_HAND;
        
        player->hand[player->cards_in_hand] = c;
        player->cards_in_hand++;
        player->hand_value += c->rank_value;
        
        if (c->rank=='A'){
            player->aces_in_hand_worth_11++;
        }
        if (player->hand_value > 21 && player->aces_in_hand_worth_11 > 0){
            player->hand_value -= 10;
            player->aces_in_hand_worth_11--;
        }

        return c;
    }
    else {
        return NULL;
    }
}

bool player_is_bet_history_empty(Player *player){
    return (player->betted_chips == 0);
}

bool player_is_bet_history_full(Player *player){
    return (player->betted_chips == MAXIMUM_BETTED_CHIPS);
}

bool game_player_bet_push(Player *player, CHIP_VALUE val){
    if (!player_is_bet_history_full(player)){
        player->bet_history[player->betted_chips] = val;
        player->bet += val;
        player->betted_chips++;
        return true;
    }
    else{
        return false;
    }
}

CHIP_VALUE player_bet_pop(Player *player){
    if (!player_is_bet_history_empty(player)){
        player->bet -= player->bet_history[player->betted_chips-1];
        player->betted_chips--;
        return player->bet_history[player->betted_chips];
    }
    else {
        return _CHIP_VALUE_NONE;
    }
}

CHIP_VALUE player_bet_peek(Player *player){
    if (!player_is_bet_history_empty(player)){
        return player->bet_history[player->betted_chips-1];
    }
    else {
        return _CHIP_VALUE_NONE;
    }
}

bool player_can_bet(Player *player){
    return player->money >= CHIP_VALUE_ONE;
}

bool player_can_double_down(Player *player){
    return player->money >= player->bet;
}

bool can_insure(Dealer *dealer){
    if (dealer->hand[0] != NULL){
        return dealer->hand[0]->rank == 'A';
    }
    else{
        return false;
    }
}