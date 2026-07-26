#include <time.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../include/game.h"

/*
RULES
Dealer hits maximum 10 times AAAA222233 because stands on 17.
Player hits maximum 5 times because 5-card charlie rule.
*/

const char suits[4] = {'C', 'D', 'H', 'S'};
const char ranks[13] = {'2', '3', '4', '5', '6', '7', '8', '9', 'T', 'J', 'Q', 'K', 'A'};
const int rank_values[13] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 10, 10, 10, 11};

Player* game_player_create(void){
    Player player = {
        {NULL}, 
        0, 
        0, 
        0, 
        PLAYER_BEGINNING_MONEY,
        0.f,
        {_CHIP_VALUE_NONE},
        0
    };
    Player *p_player = malloc(sizeof(Player));
    if (p_player == NULL){
        abort();
    }
    *p_player = player;
    return p_player;
}

void game_player_destroy(Player *p_player){
    free(p_player);
}

Dealer* game_dealer_create(void){
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

void game_dealer_destroy(Dealer *p_dealer){
    free(p_dealer);
}

Card** game_deck_create(void){
    Card** deck = calloc(52, sizeof(Card));
    if (deck == NULL){
        abort();
    }
    for (int s = 0; s < 4; s++){
        for (int r = 0; r < 13; r++){
            Card card = {
                .suit=suits[s], 
                .rank=ranks[r], 
                .rank_value=rank_values[r], 
                .location=CARD_LOCATION_DECK, 
                .face_down=true
            };
            Card *p = malloc(sizeof(Card));
            if (p==NULL){
                abort();
            }
            *p = card;
            deck[s*13+r] = p;
        }
    }
    srand(time(NULL));
    return deck;
}

void game_deck_destroy(Card** deck){
    for (int i=0; i<52; i++){
        free(deck[i]);
        deck[i] = NULL;
    }
    free(deck);
}

GameContext* game_context_create(void){
    GameContext gc = {
        GAME_STATE_NEW,
        _GAME_STATE_NONE,
        game_deck_create(),
        game_player_create(),
        game_dealer_create(),
    };
    GameContext *p_gc = malloc(sizeof(GameContext));
    if (p_gc == NULL){
        abort();
    }
    *p_gc = gc;
    return p_gc;
}

void game_context_destroy(GameContext *p_gc){
    game_deck_destroy(p_gc->deck);
    p_gc->deck = NULL;
    game_player_destroy(p_gc->player);
    p_gc->player = NULL;
    game_dealer_destroy(p_gc->dealer);
    p_gc->dealer = NULL;
    free(p_gc);
}

GAME_STATE game_context_get_game_state(GameContext *p_gc){
    return p_gc->game_state;
}

GAME_STATE game_context_get_prev_game_state(GameContext *p_gc){
    return p_gc->prev_game_state;
}

void game_context_set_game_state(GameContext *p_gc, GAME_STATE state){
    p_gc->prev_game_state=p_gc->game_state;
    p_gc->game_state=state;
}

void game_shuffle_deck(Card **deck){
    for (int i=51; i>=1; i--){
        int j = rand()%(i+1);
        Card *temp = deck[j];
        deck[j] = deck[i];
        deck[i] = temp;
    }
}

Card* game_draw_random_card(Card **deck){
    Card *c;
    do{
        c=deck[rand()%52];
    } while (c->location!=CARD_LOCATION_DECK);
    return c;
}

void game_dealer_reset(Dealer *dealer){
    for (;dealer->cards_in_hand > 0; dealer->cards_in_hand--){
        dealer->hand[dealer->cards_in_hand-1]->location = CARD_LOCATION_DECK;
        dealer->hand[dealer->cards_in_hand-1]->face_down = true;
        dealer->hand[dealer->cards_in_hand-1] = NULL;
    }
    dealer->hand_value = 0;
    dealer->aces_in_hand_worth_11 = 0;
}

void game_player_reset(Player *player){
    for (;player->cards_in_hand > 0; player->cards_in_hand--){
        player->hand[player->cards_in_hand-1]->location = CARD_LOCATION_DECK;
        player->hand[player->cards_in_hand-1]->face_down = true;
        player->hand[player->cards_in_hand-1] = NULL;
    }
    player->hand_value = 0;
    player->aces_in_hand_worth_11 = 0;
    player->bet = 0;
    player->betted_chips = 0;
}

void game_participants_reset(Dealer *dealer, Player *player){
    game_dealer_reset(dealer);
    game_player_reset(player);
}

bool game_dealer_bust(Dealer *dealer){
    return dealer->hand_value > 21 ? true : false;
}

bool game_player_bust(Player *player){
    return player->hand_value > 21 ? true : false;
}

bool game_dealer_is_blackjack(Dealer *dealer){
    return (dealer->hand_value == 21 && dealer->cards_in_hand == 2) ? true : false;
}

bool game_player_is_blackjack(Player *player){
    return (player->hand_value == 21 && player->cards_in_hand == 2) ? true : false;
}

bool game_dealer_can_hit(Dealer *dealer){
    return dealer->cards_in_hand < 10 && dealer->hand_value < 21;
}

bool game_player_can_hit(Player *player){
    return player->cards_in_hand < 5 && player->hand_value < 21;
}

bool game_dealer_is_hiding_second_card(Dealer *dealer){
    if (dealer->hand[1] != NULL){
        return dealer->hand[1]->face_down;
    }
    else{
        return true;
    }
}

void game_dealer_reveal_second_card(Dealer *dealer){
    dealer->hand[1]->face_down=false;
}

Card* game_dealer_hit(Card** deck, Dealer *dealer){
    Card *c = game_draw_random_card(deck);
    if (game_dealer_can_hit(dealer)){
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

Card* game_player_hit(Card **deck, Player *player){
    Card *c = game_draw_random_card(deck);
    if (game_player_can_hit(player)){
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

Card** game_dealer_get_hand(Dealer *dealer){
    return dealer->hand;
}

Card** game_player_get_hand(Player *player){
    return player->hand;
}

int game_dealer_get_hand_value(Dealer *dealer){
    return dealer->hand_value;
}

int game_player_get_hand_value(Player *player){
    return player->hand_value;
}

int game_player_get_cards_in_hand(Player *player){
    return player->cards_in_hand;
}

int game_dealer_get_cards_in_hand(Dealer *dealer){
    return dealer->cards_in_hand;
}

float game_player_get_money(Player *player){
    return player->money;
}

float game_player_get_bet(Player *player){
    return player->bet;
}

bool game_player_is_bet_history_empty(Player *player){
    return (player->betted_chips == 0);
}

bool game_player_is_bet_history_full(Player *player){
    return (player->betted_chips == MAXIMUM_BETTED_CHIPS);
}

bool game_player_bet_push(Player *player, CHIP_VALUE val){
    if (!game_player_is_bet_history_full(player)){
        player->bet_history[player->betted_chips] = val;
        player->bet += val;
        player->betted_chips++;
        return true;
    }
    else{
        return false;
    }
}

CHIP_VALUE game_player_bet_pop(Player *player){
    if (!game_player_is_bet_history_empty(player)){
        player->bet -= player->bet_history[player->betted_chips-1];
        player->betted_chips--;
        return player->bet_history[player->betted_chips];
    }
    else {
        return _CHIP_VALUE_NONE;
    }
}

CHIP_VALUE game_player_bet_peek(Player *player){
    if (!game_player_is_bet_history_empty(player)){
        return player->bet_history[player->betted_chips-1];
    }
    else {
        return _CHIP_VALUE_NONE;
    }
}

void game_player_add_money_to_bet(Player *player, float money){
    player->bet += money;
}

void game_player_sub_money_from_bet(Player *player, float money){
    player->bet -= money;
}

void game_player_add_money(Player *player, float money){
    player->money += money;
}

void game_player_sub_money(Player *player, float money){
    player->money -= money;
}

bool game_player_can_bet(Player *player){
    return game_player_get_money(player) >= CHIP_VALUE_ONE;
}

bool game_player_can_double_down(Player *player){
    return game_player_get_money(player) >= game_player_get_bet(player);
}

bool game_can_insure(Dealer *dealer){
    if (dealer->hand[0] != NULL){
        return dealer->hand[0]->rank == 'A';
    }
    else{
        return false;
    }
}