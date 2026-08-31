#include <time.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../../include/stb_ds.h"
#include "../../include/game/card_constants.h"
#include "../../include/game/game_constants.h"
#include "../../include/ui/button_constants.h"
#include "../../include/game/game.h"

/*
RULES
Hits maximum 12 times AAAA22223333, 22 which is bust.
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
        {NULL},
        0,
        0,
        0,
        PLAYER_BEGINNING_MONEY,
        0.f,
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

cheque_data_hash* cheque_data_map_create(void){
    cheque_data_hash* cheque_data_map = NULL;
    cheque_data cdata = {
        TEXTURE_ID_WHITE_CHEQUE, 
        TEXTURE_ID_WHITE_BUTTON_SPRITESHEET,
        CHIP_BUTTON_X(0),
        CHIP_BUTTON_Y(0)
    };
    hmput(cheque_data_map, CHEQUE_VALUE_ONE, cdata);
    cdata = (cheque_data){
        TEXTURE_ID_RED_CHEQUE, 
        TEXTURE_ID_RED_BUTTON_SPRITESHEET,
        CHIP_BUTTON_X(1),
        CHIP_BUTTON_Y(1)
    };
    hmput(cheque_data_map, CHEQUE_VALUE_FIVE, cdata);
    cdata = (cheque_data){
        TEXTURE_ID_BLUE_CHEQUE, 
        TEXTURE_ID_BLUE_BUTTON_SPRITESHEET,
        CHIP_BUTTON_X(2),
        CHIP_BUTTON_Y(2)
    };
    hmput(cheque_data_map, CHEQUE_VALUE_TEN, cdata);
    cdata = (cheque_data){
        TEXTURE_ID_GREEN_CHEQUE, 
        TEXTURE_ID_GREEN_BUTTON_SPRITESHEET,
        CHIP_BUTTON_X(3),
        CHIP_BUTTON_Y(3)
    };
    hmput(cheque_data_map, CHEQUE_VALUE_TWENTY_FIVE, cdata);
    cdata = (cheque_data){
        TEXTURE_ID_BLACK_CHEQUE, 
        TEXTURE_ID_BLACK_BUTTON_SPRITESHEET,
        CHIP_BUTTON_X(4),
        CHIP_BUTTON_Y(4)
    };
    hmput(cheque_data_map, CHEQUE_VALUE_HUNDRED, cdata);
    cdata = (cheque_data){
        TEXTURE_ID_PURPLE_CHEQUE, 
        TEXTURE_ID_PURPLE_BUTTON_SPRITESHEET,
        CHIP_BUTTON_X(5),
        CHIP_BUTTON_Y(5)
    };
    hmput(cheque_data_map, CHEQUE_VALUE_FIVE_HUNDRED, cdata);
    cdata = (cheque_data){
        TEXTURE_ID_YELLOW_CHEQUE, 
        TEXTURE_ID_YELLOW_BUTTON_SPRITESHEET,
        CHIP_BUTTON_X(6),
        CHIP_BUTTON_Y(6)
    };
    hmput(cheque_data_map, CHEQUE_VALUE_ONE_K, cdata);
    cdata = (cheque_data){
        TEXTURE_ID_ORANGE_CHEQUE, 
        TEXTURE_ID_ORANGE_BUTTON_SPRITESHEET,
        CHIP_BUTTON_X(7),
        CHIP_BUTTON_Y(7)
    };
    hmput(cheque_data_map, CHEQUE_VALUE_FIVE_K, cdata);
    cdata = (cheque_data){
        TEXTURE_ID_REDBLUE_CHEQUE, 
        TEXTURE_ID_REDBLUE_BUTTON_SPRITESHEET,
        CHIP_BUTTON_X(8),
        CHIP_BUTTON_Y(8)
    };
    hmput(cheque_data_map, CHEQUE_VALUE_TWENTY_FIVE_K, cdata);
    cdata = (cheque_data){
        TEXTURE_ID_GOLD_CHEQUE, 
        TEXTURE_ID_GOLD_BUTTON_SPRITESHEET,
        CHIP_BUTTON_X(9),
        CHIP_BUTTON_Y(9)
    };
    hmput(cheque_data_map, CHEQUE_VALUE_HUNDRED_K, cdata);
    return cheque_data_map;
}

void cheque_data_map_destroy(cheque_data_hash *cheque_data_map){
    hmfree(cheque_data_map);
}

GameContext* game_context_create(void){
    GameContext game_ctx = {
        _GAME_STATE_NONE,
        _GAME_STATE_NONE,
        cheque_ring_buffer_create(MAXIMUM_ALIVE_CHEQUES),
        cheque_data_map_create(),
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
    dealer_destroy(game_ctx->dealer);
    game_ctx->dealer = NULL;
    player_destroy(game_ctx->player);
    game_ctx->player = NULL;
    free(game_ctx->deck_top_index_ptr);
    deck_destroy(game_ctx->deck);
    game_ctx->deck = NULL;
    cheque_data_map_destroy(game_ctx->cheque_data_map);
    cheque_ring_buffer_destroy(game_ctx->cheque_ring_buffer);
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

void game_reset_dealer(GameContext *game_ctx){
    Dealer *dealer = game_ctx->dealer;
    for (;dealer->cards_in_hand > 0; dealer->cards_in_hand--){
        Card** dhand = dealer->hand;
        int cindex = dealer->cards_in_hand - 1;
        dhand[cindex]->location = CARD_LOCATION_DECK;
        dhand[cindex]->face_down = true;
        dhand[cindex]->rect.pos.x = DECK_ORIGIN_X - (51 - *game_ctx->deck_top_index_ptr);
        dhand[cindex]->rect.pos.y = DECK_ORIGIN_Y + (51 - *game_ctx->deck_top_index_ptr);
        dhand[cindex] = NULL;
    }
    dealer->hand_value = 0;
    dealer->aces_in_hand_worth_11 = 0;
}

void game_reset_player(GameContext *game_ctx){
    Player *player = game_ctx->player;
    for (;player->cards_in_hand > 0; player->cards_in_hand--){
        Card** phand = player->hand;
        int cindex = player->cards_in_hand - 1;
        phand[cindex]->location = CARD_LOCATION_DECK;
        phand[cindex]->face_down = true;
        phand[cindex]->rect.pos.x = DECK_ORIGIN_X - (51 - *game_ctx->deck_top_index_ptr);
        phand[cindex]->rect.pos.y = DECK_ORIGIN_Y + (51 - *game_ctx->deck_top_index_ptr);
        phand[cindex] = NULL;
    }
    player->hand_value = 0;
    player->aces_in_hand_worth_11 = 0;
    for (;player->cards_in_split_hand > 0; player->cards_in_split_hand--){
        Card** split_phand = player->split_hand;
        int cindex = player->cards_in_split_hand - 1;
        split_phand[cindex]->location = CARD_LOCATION_DECK;
        split_phand[cindex]->face_down = true;
        split_phand[cindex]->rect.pos.x = DECK_ORIGIN_X - (51 - *game_ctx->deck_top_index_ptr);
        split_phand[cindex]->rect.pos.y = DECK_ORIGIN_Y + (51 - *game_ctx->deck_top_index_ptr);
        split_phand[cindex] = NULL;
    }
    player->split_hand_value = 0;
    player->aces_in_split_hand_worth_11 = 0;
    player->bet = 0;
    player->split_bet = 0;
    if (player->bet_count > 0) {
        arrdeln(player->bet_stack, 0, player->bet_count);
    }
    player->bet_count = 0;
}

void game_reset(GameContext *game_ctx){
    game_reset_dealer(game_ctx);
    game_reset_player(game_ctx);
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

Card* player_hit_split(Card *deck, int *deck_top_index_ptr, Player *player){
    if (can_hit(player->cards_in_split_hand, player->split_hand_value)){
        Card *c = draw(deck, deck_top_index_ptr);
        c->location = CARD_LOCATION_PLAYER_SPLIT_HAND;
        
        player->split_hand[player->cards_in_split_hand] = c;
        player->cards_in_split_hand++;
        player->split_hand_value += c->rank_value;
        
        if (c->rank=='A'){
            player->aces_in_split_hand_worth_11++;
        }
        if (player->split_hand_value > 21 && player->aces_in_split_hand_worth_11 > 0){
            player->split_hand_value -= 10;
            player->aces_in_split_hand_worth_11--;
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

void player_bet_push(Player *player, CHEQUE_VALUE val){
    player->bet += arrput(player->bet_stack, val);
    player->bet_count++;
}

CHEQUE_VALUE player_bet_pop(Player *player){
    if (!player_is_bet_history_empty(player)){
        CHEQUE_VALUE val = arrpop(player->bet_stack);
        player->bet -= val;
        player->bet_count--;
        return val;
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

void game_split(GameContext *game_ctx){
    game_ctx->player->split_hand[0] = game_ctx->player->hand[1];
    game_ctx->player->hand[1] = NULL;
    game_ctx->player->cards_in_hand--;
    game_ctx->player->hand_value -= game_ctx->player->split_hand[0]->rank_value;
    game_ctx->player->cards_in_split_hand++;
    game_ctx->player->split_hand_value += game_ctx->player->split_hand[0]->rank_value;
    if (game_ctx->player->split_hand[0]->rank == 'A'){
        game_ctx->player->aces_in_split_hand_worth_11++;
    }
    game_ctx->dealer->hand[0]->rect.pos.y = HAND_SPLITTING_Y_DEALER;
    game_ctx->dealer->hand[1]->rect.pos.y = HAND_SPLITTING_Y_DEALER;
    game_ctx->player->hand[0]->rect.pos = (vec2){HAND_ORIGIN_X, HAND_SPLITTING_Y_PLAYER};
    game_ctx->player->split_hand[0]->rect.pos = (vec2){HAND_ORIGIN_X, SPLIT_HAND_SPLITTING_Y_PLAYER};
    game_ctx->player->split_bet = game_ctx->player->bet;
    game_ctx->player->money -= game_ctx->player->split_bet;
}