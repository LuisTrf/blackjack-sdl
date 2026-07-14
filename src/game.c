#include <time.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../include/game.h"

/*
RULES
Dealer hits maximum 10 times AAAA222233 because stands on 17.
Player hits maximum 5 times because 5-card charlie rule.
*/

typedef struct Player {
    Card *hand[PLAYER_MAXIMUM_HAND_SIZE];
    unsigned char hand_value;
    unsigned char cards_in_hand;
    unsigned char aces_in_hand_worth_11;
    double money;
    double bet;
    CHIP_VALUE bet_history[MAXIMUM_BETTED_CHIPS];
    unsigned int betted_chips;
} Player;

typedef struct Dealer{
    Card *hand[DEALER_MAXIMUM_HAND_SIZE];
    unsigned char hand_value;
    unsigned char cards_in_hand;
    unsigned char aces_in_hand_worth_11;
} Dealer;

const char suits[4] = {'C', 'D', 'H', 'S'};
const char ranks[13] = {'2', '3', '4', '5', '6', '7', '8', '9', 'T', 'J', 'Q', 'K', 'A'};
const int rank_values[13] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 10, 10, 10, 11};

static GAME_STATE game_state = GAME_STATE_NEW;
static GAME_STATE prev_game_state;
static Card *deck[52];
static Player player = {.hand={NULL}, .hand_value=0, .cards_in_hand=0, .aces_in_hand_worth_11=0, .money=PLAYER_BEGINNING_MONEY, .bet=0, 
    .bet_history={_CHIP_VALUE_NONE}, .betted_chips=0};
static Dealer dealer = {.hand={NULL}, .hand_value=0, .cards_in_hand=0, .aces_in_hand_worth_11=0};

GAME_STATE get_game_state(void){
    return game_state;
}

GAME_STATE get_prev_game_state(void){
    return prev_game_state;
}

void set_game_state(GAME_STATE state){
    prev_game_state=game_state;
    game_state=state;
}

void allocate_deck_memory(void){
    for (int s=0; s<4; s++){
        for (int r=0; r<13; r++){
            Card card = {.suit=suits[s], .rank=ranks[r], .rank_value=rank_values[r], .location=CARD_LOCATION_DECK, .face_down=true};
            Card *p = malloc(sizeof(Card));
            if (p==NULL){
                abort();
            }
            *p = card;
            deck[s*13+r] = p;
        }
    }
    srand(time(NULL));
}

void free_deck_memory(void){
    for (int i=0; i<52; i++){
        free(deck[i]);
        deck[i] = NULL;
    }
}

void shuffle_deck(void){
    for (int i=51; i>=1; i--){
        int j = rand()%(i+1);
        Card *temp = deck[j];
        deck[j] = deck[i];
        deck[i] = temp;
    }
}

Card* draw_random_card(void){
    Card *c;
    do{
        c=deck[rand()%52];
    } while (c->location!=CARD_LOCATION_DECK);
    return c;
}

void reset_dealer(void){
    for (;dealer.cards_in_hand>0; dealer.cards_in_hand--){
        dealer.hand[dealer.cards_in_hand-1]->location = CARD_LOCATION_DECK;
        dealer.hand[dealer.cards_in_hand-1]->face_down = true;
        dealer.hand[dealer.cards_in_hand-1] = NULL;
    }
    dealer.hand_value = 0;
    dealer.aces_in_hand_worth_11 = 0;
}

void reset_player(void){
    for (;player.cards_in_hand>0; player.cards_in_hand--){
        player.hand[player.cards_in_hand-1]->location = CARD_LOCATION_DECK;
        player.hand[player.cards_in_hand-1]->face_down = true;
        player.hand[player.cards_in_hand-1] = NULL;
    }
    player.hand_value = 0;
    player.aces_in_hand_worth_11 = 0;
    player.bet = 0;
    player.betted_chips = 0;
}

void reset_game_state(void){
    reset_dealer();
    reset_player();
}

bool dealer_bust(void){
    return dealer.hand_value > 21 ? true : false;
}

bool player_bust(void){
    return player.hand_value > 21 ? true : false;
}

bool dealer_blackjack(void){
    return (dealer.hand_value == 21 && dealer.cards_in_hand == 2) ? true : false;
}

bool player_blackjack(void){
    return (player.hand_value == 21 && player.cards_in_hand == 2) ? true : false;
}

bool dealer_can_hit(void){
    return dealer.cards_in_hand < 10 && dealer.hand_value < 21;
}

bool player_can_hit(void){
    return player.cards_in_hand < 5 && player.hand_value < 21;
}

bool is_second_dealer_card_hidden(void){
    if (dealer.hand[1]!=NULL){
        return dealer.hand[1]->face_down;
    }
    else{
        return true;
    }
}

void reveal_second_dealer_card(void){
    dealer.hand[1]->face_down=false;
}

Card* dealer_hit(void){
    Card *c = draw_random_card();
    if (dealer_can_hit()){
        c->location=CARD_LOCATION_DEALER_HAND;
        dealer.hand[dealer.cards_in_hand] = c;
        dealer.cards_in_hand++;
        dealer.hand_value += c->rank_value;
        
        if (c->rank=='A'){
            dealer.aces_in_hand_worth_11++;
        }
        if (dealer.hand_value > 21 && dealer.aces_in_hand_worth_11 > 0){
            dealer.hand_value -= 10;
            dealer.aces_in_hand_worth_11--;
        }

        return c;
    }
    else {
        return NULL;
    }
}

Card* player_hit(void){
    Card *c = draw_random_card();
    if (player_can_hit()){
        c->location=CARD_LOCATION_PLAYER_HAND;
        player.hand[player.cards_in_hand] = c;
        player.cards_in_hand++;
        player.hand_value += c->rank_value;
        
        if (c->rank=='A'){
            player.aces_in_hand_worth_11++;
        }
        if (player.hand_value > 21 && player.aces_in_hand_worth_11 > 0){
            player.hand_value -= 10;
            player.aces_in_hand_worth_11--;
        }

        return c;
    }
    else {
        return NULL;
    }
}

Card** get_dealer_hand(void){
    return dealer.hand;
}

Card** get_player_hand(void){
    return player.hand;
}

int get_dealer_hand_value(void){
    return dealer.hand_value;
}

int get_player_hand_value(void){
    return player.hand_value;
}

int get_player_cards_in_hand(void){
    return player.cards_in_hand;
}

int get_dealer_cards_in_hand(void){
    return dealer.cards_in_hand;
}

double get_player_money(void){
    return player.money;
}

double get_player_bet(void){
    return player.bet;
}

bool is_bet_history_empty(void){
    return (player.betted_chips==0);
}

bool is_bet_history_full(void){
    return (player.betted_chips==MAXIMUM_BETTED_CHIPS);
}

bool push_bet(CHIP_VALUE val){
    if (!is_bet_history_full()){
        player.bet_history[player.betted_chips] = val;
        player.bet += val;
        player.betted_chips++;
        return true;
    }
    else{
        return false;
    }
}

CHIP_VALUE pop_bet(void){
    if (!is_bet_history_empty()){
        player.bet -= player.bet_history[player.betted_chips-1];
        player.betted_chips--;
        return player.bet_history[player.betted_chips];
    }
    else {
        return _CHIP_VALUE_NONE;
    }
}

CHIP_VALUE peek_bet(void){
    if (!is_bet_history_empty()){
        return player.bet_history[player.betted_chips-1];
    }
    else {
        return _CHIP_VALUE_NONE;
    }
}

void add_to_player_bet(double money){
    player.bet += money;
}

void sub_from_player_bet(double money){
    player.bet += money;
}

void add_to_player_money(double money){
    player.money += money;
}

void sub_from_player_money(double money){
    player.money -= money;
}

bool can_player_bet(void){
    return get_player_money() >= CHIP_VALUE_ONE;
}

bool can_double_down(void){
    return get_player_money() >= get_player_bet();
}

bool can_insure(void){
    if (dealer.hand[0]!=NULL){
        return dealer.hand[0]->rank=='A';
    }
    else{
        return false;
    }
}