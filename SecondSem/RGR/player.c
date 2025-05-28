#include "player.h"
#include <stdio.h>

void initialize_players(Player players[MAX_PLAYERS]) {
    for (int i = 0; i < MAX_PLAYERS; i++) {
        players[i].hand[0].value = 0;
        players[i].hand[1].value = 0;
        players[i].money = STARTING_MONEY;
        players[i].current_bet = 0;
        players[i].status = PLAYING;
        players[i].is_human = (i == 0); // First player is human
        players[i].has_acted = 0;
    }
}

void place_blinds(Player players[MAX_PLAYERS]) {
    // Small blind
    players[1].current_bet = SMALL_BLIND;
    players[1].money -= SMALL_BLIND;

    // Big blind
    players[2].current_bet = BIG_BLIND;
    players[2].money -= BIG_BLIND;
}

void reset_player_bets(Player players[MAX_PLAYERS]) {
    for (int i = 0; i < MAX_PLAYERS; i++) {
        players[i].current_bet = 0;
        players[i].status = players[i].status == ALL_IN ? ALL_IN : PLAYING;
        players[i].has_acted = 0;
    }
}