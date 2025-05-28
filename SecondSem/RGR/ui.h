#ifndef UI_H
#define UI_H

#include "game.h"

void display_game_state(const Game* game);
void display_player_hand(const Player* player);
void display_community_cards(const Game* game);
void clear_screen();
void display_winner(const Game* game, int winner_index, HandRank rank);
void get_player_action(Game* game);
void wait_for_key_press();

#endif