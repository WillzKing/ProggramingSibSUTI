#ifndef PLAYER_H
#define PLAYER_H

#include "deck.h"

#define MAX_PLAYERS 6 // 1 human + 5 bots
#define STARTING_MONEY 1000
#define SMALL_BLIND 10
#define BIG_BLIND 20

typedef enum {
    FOLDED,
    PLAYING,
    ALL_IN
} PlayerStatus;

typedef struct {
    Card hand[2];
    int money;
    int current_bet;
    PlayerStatus status;
    int is_human;
    int has_acted;
} Player;


void initialize_players(Player players[MAX_PLAYERS]);
void place_blinds(Player players[MAX_PLAYERS]);
void reset_player_bets(Player players[MAX_PLAYERS]);

#endif