#ifndef GAME_H
#define GAME_H

#include "player.h"
#include "deck.h"

#define MAX_COMMUNITY_CARDS 5

typedef enum {
    PREFLOP,
    FLOP,
    TURN,
    RIVER,
    SHOWDOWN
} GameState;

typedef struct {
    Player players[MAX_PLAYERS];
    Deck deck;
    Card community_cards[MAX_COMMUNITY_CARDS];
    int community_cards_count;
    int pot;
    int current_player;
    int dealer_position;
    GameState state;
} Game;

typedef enum {
    HIGH_CARD,
    PAIR,
    TWO_PAIR,
    THREE_OF_A_KIND,
    STRAIGHT,
    FLUSH,
    FULL_HOUSE,
    FOUR_OF_A_KIND,
    STRAIGHT_FLUSH,
    ROYAL_FLUSH
} HandRank;

typedef struct {
    Card cards[5];
    HandRank rank;
} BestHand;

BestHand evaluate_best_hand(Card player_hand[2], Card community_cards[5], int community_count);
void determine_winner(Game* game);

int get_current_max_bet(const Game* game);  // Добавлено
void determine_winner(Game* game);          // Добавлено
void initialize_game(Game* game);
void deal_hole_cards(Game* game);
void deal_flop(Game* game);
void deal_turn(Game* game);
void deal_river(Game* game);
int is_round_complete(const Game* game);
void advance_round(Game* game);
int is_game_over(const Game* game);
void pause();

#endif