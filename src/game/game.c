#include <time.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../../include/game/card_constants.h"
#include "../../include/game/game_constants.h"
#include "../../include/game/game.h"

/*
RULES
Hits maximum 12 times AAAA22223333, 22 which is bust.
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
        NULL
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

Card* deck_create(void){
    Card* deck = calloc(52, sizeof(Card));
    if (deck == NULL){
        abort();
    }
    for (int s = 0; s < 4; s++){
        for (int r = 0; r < 13; r++){
            Card card = {
                .rect = {
                    .pos = {
                        .x=DECK_ORIGIN_X - (51 - (s*13 + r)),
                        .y=DECK_ORIGIN_Y + (51 - (s*13 + r)),
                    },
                    .width=CARD_WIDTH,
                    .height=CARD_HEIGHT,
                    .visible=true,
                },
                .suit=SUITS[s], 
                .rank=RANKS[r], 
                .rank_value=RANK_VALUES[r], 
                .location=CARD_LOCATION_DECK, 
                .face_down=true
            };
            deck[s*13 + r] = card;
        }
    }
    srand(time(NULL));
    return deck;
}

void deck_destroy(Card* deck){
    free(deck);
}

ChequeRingBuffer* cheque_ring_buffer_create(int size){
    Cheque *arr = calloc(size, sizeof(Cheque));
    if (arr == NULL){
        abort();
    }
    ChequeRingBuffer crb = {size, 0, 0, 0, arr};
    ChequeRingBuffer *p_crb = malloc(sizeof(ChequeRingBuffer));
    if (p_crb == NULL){
        abort();
    }
    *p_crb = crb;
    return p_crb;
}

void cheque_ring_buffer_destroy(ChequeRingBuffer *cheque_ring_buffer){
    free(cheque_ring_buffer);
}

bool cheque_ring_buffer_full(ChequeRingBuffer *cheque_ring_buffer){
    return cheque_ring_buffer->count == cheque_ring_buffer->size;
}

bool cheque_ring_buffer_empty(ChequeRingBuffer *cheque_ring_buffer){
    return cheque_ring_buffer->count == 0;
}

void cheque_ring_buffer_enqueue(ChequeRingBuffer *cheque_ring_buffer, Cheque cheque){
    if (cheque_ring_buffer_full(cheque_ring_buffer)){
        return;
    }
    cheque_ring_buffer->arr[cheque_ring_buffer->tail] = cheque;
    cheque_ring_buffer->tail = (cheque_ring_buffer->tail + 1) % cheque_ring_buffer->size;
    ++cheque_ring_buffer->count;
}

Cheque cheque_ring_buffer_dequeue(ChequeRingBuffer *cheque_ring_buffer){
    if (cheque_ring_buffer_empty(cheque_ring_buffer)){
        return (Cheque){{0, 0, 0, 0, false}, _CHEQUE_VALUE_NONE, TEXTURE_ID_NULL};
    }
    Cheque cheque = cheque_ring_buffer->arr[cheque_ring_buffer->head];
    cheque_ring_buffer->head = (cheque_ring_buffer->head + 1) % cheque_ring_buffer->size;
    --cheque_ring_buffer->count;
    return cheque;
}

GameContext* game_context_create(void){
    GameContext game_ctx = {
        GAME_STATE_NEW,
        _GAME_STATE_NONE,
        cheque_ring_buffer_create(MAXIMUM_ALIVE_CHEQUES),
        deck_create(),
        malloc(sizeof(int)),
        dealer_create(),
        player_create(),
    };
    if (game_ctx.deck_top_index_ptr == NULL){
        abort();
    }
    *game_ctx.deck_top_index_ptr = 51;
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

void deck_shuffle(Card *deck, int *deck_top_index_ptr){
    for (int i = *deck_top_index_ptr; i > 0; i--){
        int j = rand()%(i+1);
        Card temp = deck[j];
        deck[j] = deck[i];
        deck[i] = temp;
        deck[j].rect.pos.x = DECK_ORIGIN_X - (51 - j);
        deck[j].rect.pos.y = DECK_ORIGIN_Y + (51 - j);
        deck[i].rect.pos.x = DECK_ORIGIN_X - (51 - i);
        deck[i].rect.pos.y = DECK_ORIGIN_Y + (51 - i);
    }
}

bool is_deck_empty(int deck_top_index){
    return (deck_top_index == -1);
}

bool is_deck_full(int deck_top_index){
    return (deck_top_index == 51);
}

Card* draw(Card *deck, int *deck_top_index){
    if (!is_deck_empty(*deck_top_index)){
        Card *c = &deck[(*deck_top_index)--];
        return c;
    }
    else {
        return NULL;
    }
}

Card* draw_random_card(Card *deck, int *deck_top_index){
    Card *c;
    do{
        c = &deck[rand()%(*deck_top_index-1)];
    } while (c->location!=CARD_LOCATION_DECK);
    return c;
}

void game_reset(GameContext *game_ctx){
    for (;game_ctx->dealer->cards_in_hand > 0; game_ctx->dealer->cards_in_hand--){
        game_ctx->dealer->hand[game_ctx->dealer->cards_in_hand-1]->location = CARD_LOCATION_DECK;
        game_ctx->dealer->hand[game_ctx->dealer->cards_in_hand-1]->face_down = true;
        game_ctx->dealer->hand[game_ctx->dealer->cards_in_hand-1]->rect.pos.x = DECK_ORIGIN_X - (51 - *game_ctx->deck_top_index_ptr);
        game_ctx->dealer->hand[game_ctx->dealer->cards_in_hand-1]->rect.pos.y = DECK_ORIGIN_Y + (51 - *game_ctx->deck_top_index_ptr);
        game_ctx->dealer->hand[game_ctx->dealer->cards_in_hand-1] = NULL;
    }
    game_ctx->dealer->hand_value = 0;
    game_ctx->dealer->aces_in_hand_worth_11 = 0;

    for (;game_ctx->player->cards_in_hand > 0; game_ctx->player->cards_in_hand--){
        game_ctx->player->hand[game_ctx->player->cards_in_hand-1]->location = CARD_LOCATION_DECK;
        game_ctx->player->hand[game_ctx->player->cards_in_hand-1]->face_down = true;
        game_ctx->player->hand[game_ctx->player->cards_in_hand-1]->rect.pos.x = DECK_ORIGIN_X - (51 - *game_ctx->deck_top_index_ptr);
        game_ctx->player->hand[game_ctx->player->cards_in_hand-1]->rect.pos.y = DECK_ORIGIN_Y + (51 - *game_ctx->deck_top_index_ptr);
        game_ctx->player->hand[game_ctx->player->cards_in_hand-1] = NULL;
    }
    game_ctx->player->hand_value = 0;
    game_ctx->player->aces_in_hand_worth_11 = 0;
    game_ctx->player->bet = 0;
    game_ctx->player->bet_count = 0;

    *game_ctx->deck_top_index_ptr = 51;
}

bool bust(int hand_value){
    return hand_value > 21;
}

bool blackjack(int cards_in_hand, int hand_value){
    return cards_in_hand == 2 && hand_value == 21;
}

bool can_hit(int cards_in_hand, int hand_value){
    return cards_in_hand < 12 && hand_value < 21;
}

bool dealer_is_hiding_second_card(Dealer *dealer){
    if (dealer->hand[1] != NULL){
        return dealer->hand[1]->face_down;
    }
    else{
        return true;
    }
}

bool is_second_dealer_card(Dealer *dealer, Card *card){
    return (card == dealer->hand[1]);
}

void dealer_reveal_second_card(Dealer *dealer){
    dealer->hand[1]->face_down = false;
}

void flip_card(Card *card, bool is_second_dealer_card, bool dealer_is_hiding_second_card){
    if (!is_second_dealer_card || !dealer_is_hiding_second_card){
        card->face_down = false;
    }
}

Card* dealer_hit(Card *deck, int *deck_top_index_ptr, Dealer *dealer){
    if (can_hit(dealer->cards_in_hand, dealer->hand_value)){
        Card *c = draw(deck, deck_top_index_ptr);
        c->location = CARD_LOCATION_DEALER_HAND;

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

Card* player_hit(Card *deck, int *deck_top_index_ptr, Player *player){
    if (can_hit(player->cards_in_hand, player->hand_value)){
        Card *c = draw(deck, deck_top_index_ptr);
        c->location = CARD_LOCATION_PLAYER_HAND;
        
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
    return (player->bet_count == 0);
}

bool player_is_bet_history_full(Player *player){
    return (player->bet_count == MAXIMUM_BETTED_CHIPS);
}

bool game_player_bet_push(Player *player, CHEQUE_VALUE val){
    if (!player_is_bet_history_full(player)){
        player->bet_stack[player->bet_count] = val;
        player->bet += val;
        player->bet_count++;
        return true;
    }
    else{
        return false;
    }
}

CHEQUE_VALUE player_bet_pop(Player *player){
    if (!player_is_bet_history_empty(player)){
        player->bet -= player->bet_stack[--player->bet_count];
        return player->bet_stack[player->bet_count];
    }
    else {
        return _CHEQUE_VALUE_NONE;
    }
}

CHEQUE_VALUE player_bet_peek(Player *player){
    if (!player_is_bet_history_empty(player)){
        return player->bet_stack[player->bet_count-1];
    }
    else {
        return _CHEQUE_VALUE_NONE;
    }
}

bool player_can_bet(Player *player){
    return player->money >= CHEQUE_VALUE_ONE;
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