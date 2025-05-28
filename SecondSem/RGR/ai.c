#include "ai.h"
#include <stdlib.h>
#include <time.h>
#include <windows.h>

void make_ai_decision(Game* game, int player_index) {
    Player* player = &game->players[player_index];

    if (player->status != PLAYING || player->money <= 0) {
        return;
    }

    LARGE_INTEGER seed;
    QueryPerformanceCounter(&seed);
    srand(seed.QuadPart);
    // Simple AI logic
    int current_max_bet = get_current_max_bet(game);
    int random_choice = rand() % 100;

    if (random_choice < 10) {
        // 10% chance to fold
        player->status = FOLDED;
    }
    else if (random_choice < 50) {
        // 40% chance to call/check
        int amount_to_call = current_max_bet - player->current_bet;
        if (amount_to_call >= player->money) {
            player->current_bet += player->money;
            player->money = 0;
            player->status = ALL_IN;
        }
        else {
            player->current_bet = current_max_bet;
            player->money -= amount_to_call;
        }
    }
    else {
        // 50% chance to raise
        int amount_to_call = current_max_bet - player->current_bet;
        int raise_amount = (rand() % 5 + 1) * BIG_BLIND;

        if (amount_to_call + raise_amount >= player->money) {
            player->current_bet += player->money;
            player->money = 0;
            player->status = ALL_IN;
        }
        else {
            player->current_bet = current_max_bet + raise_amount;
            player->money -= (amount_to_call + raise_amount);
        }
    }

    player->has_acted = 1;
    game->pot += player->current_bet;
}