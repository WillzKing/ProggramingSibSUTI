#include "game.h"
#include "ui.h"
#include "ai.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
    srand((unsigned int)time(NULL)); // Инициализация рандома здесь

    Game game;
    initialize_players(game.players); // Инициализация денег только один раз
    game.dealer_position = 0;

    while (!is_game_over(&game)) {
        initialize_game(&game);

        while (game.state != SHOWDOWN) {
            display_game_state(&game);
            display_player_hand(&game.players[0]);

            if (game.players[game.current_player].is_human) {
                get_player_action(&game);
            }
            else {
                make_ai_decision(&game, game.current_player);
            }

            do {
                game.current_player = (game.current_player + 1) % MAX_PLAYERS;
            } while (game.players[game.current_player].status != PLAYING ||
                game.players[game.current_player].money <= 0);

            if (is_round_complete(&game)) {
                advance_round(&game);
                display_game_state(&game);
            }
        }
    }

    printf("Game over! You ran out of money.\n");
    return 0;
}