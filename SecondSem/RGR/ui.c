#include "ui.h"
#include "game.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#ifdef _WIN32
#include <windows.h>
#include <conio.h> // Для _getch()
#define CLEAR_SCREEN() system("cls")
#define PAUSE() Sleep(1000)
#else
#include <unistd.h>
#define CLEAR_SCREEN() system("clear")
#define PAUSE() sleep(1)
#endif

void clear_screen() {
    CLEAR_SCREEN();
}

void pause() {
    PAUSE();
}

const char* hand_rank_to_string(HandRank rank) {
    static const char* ranks[] = {
        "High Card",
        "Pair",
        "Two Pairs",
        "Three of a Kind",
        "Straight",
        "Flush",
        "Full House",
        "Four of a Kind",
        "Straight Flush",
        "Royal Flush"
    };
    return ranks[rank];
}

void display_hand(const Card hand[], int count) {
    for (int i = 0; i < count; i++) {
        printf("%s", card_to_string(hand[i]));
        if (i < count - 1) printf(", ");
    }
}

void display_game_state(const Game* game) {
    clear_screen();
    printf("=== TEXAS HOLDEM POKER ===\n\n");
    printf("Pot: $%d\n\n", game->pot);

    printf("Community cards: ");
    if (game->community_cards_count == 0) {
        printf("None yet");
    }
    else {
        display_hand(game->community_cards, game->community_cards_count);
    }
    printf("\n\n");

    for (int i = 0; i < MAX_PLAYERS; i++) {
        printf("Player %d: $%d", i, game->players[i].money);

        if (game->players[i].current_bet > 0) {
            printf(" (bet: $%d)", game->players[i].current_bet);
        }

        if (game->players[i].status == FOLDED) {
            printf(" - FOLDED");
        }
        else if (game->players[i].status == ALL_IN) {
            printf(" - ALL IN");
        }

        if (i == game->current_player) {
            printf(" <-- ACTIVE");
        }

        printf("\n");
    }
    printf("\n");
}

void display_player_hand(const Player* player) {
    if (player->is_human && player->status != FOLDED) {
        printf("Your hand: ");
        display_hand(player->hand, 2);
        printf("\n");
    }
}

int get_integer_input(int min, int max) {
    char input[100];
    int value;

    while (1) {
        if (fgets(input, sizeof(input), stdin) != NULL) {
            if (sscanf(input, "%d", &value) == 1 && value >= min && value <= max) {
                return value;
            }
        }
        printf("Invalid input. Enter number %d-%d: ", min, max);
    }
}

void get_player_action(Game* game) {
    Player* player = &game->players[0];
    if (!player->is_human || player->status != PLAYING || player->money <= 0) {
        return;
    }

    int current_max = get_current_max_bet(game);
    int to_call = current_max - player->current_bet;

    printf("\nAction required:\n");
    printf("1. Fold\n");
    if (to_call == 0) {
        printf("2. Check\n");
    }
    else {
        printf("2. Call ($%d)\n", to_call);
    }
    printf("3. Raise\n");

    int choice = get_integer_input(1, 3);

    switch (choice) {
    case 1: // Fold
        player->status = FOLDED;
        printf("\nYou folded.\n");
        break;

    case 2: // Call/Check
        if (to_call == 0) {
            printf("\nYou checked.\n");
        }
        else {
            if (to_call >= player->money) {
                printf("\nYou go all-in with $%d!\n", player->money);
                game->pot += player->money;
                player->current_bet += player->money;
                player->money = 0;
                player->status = ALL_IN;
            }
            else {
                printf("\nYou call $%d.\n", to_call);
                player->money -= to_call;
                player->current_bet += to_call;
                game->pot += to_call;
            }
        }
        break;

    case 3: // Raise
    {
        int min_raise = (to_call == 0) ? BIG_BLIND : to_call;
        int max_raise = player->money;

        printf("\nEnter raise amount ($%d-$%d): ", min_raise, max_raise);
        int raise = get_integer_input(min_raise, max_raise);

        int total = to_call + raise;
        player->money -= total;
        player->current_bet += total;
        game->pot += total;

        printf("\nYou raise to $%d\n", player->current_bet);
        break;
    }
    }

    player->has_acted = 1;
    pause();
}

void wait_for_key_press() {
#ifdef _WIN32
    printf("\nPress any key to continue...");
    _getch();
#else
    printf("\nPress Enter to continue...");
    while (getchar() != '\n');
#endif
}

void display_winner(const Game* game, int winner_index, HandRank rank) {
    clear_screen();
    printf("=== SHOWDOWN RESULTS ===\n\n");

    // Показываем карты всех игроков
    for (int i = 0; i < MAX_PLAYERS; i++) {
        if (game->players[i].status != FOLDED) {
            printf("Player %d: %s, %s", i,
                card_to_string(game->players[i].hand[0]),
                card_to_string(game->players[i].hand[1]));

            if (i == winner_index) printf(" - WINNER");
            printf("\n");
        }
        else {
            printf("Player %d: Folded\n", i);
        }
    }

    printf("\nCommunity cards: ");
    for (int i = 0; i < game->community_cards_count; i++) {
        printf("%s | ", card_to_string(game->community_cards[i]));
    }

    printf("\n\nPlayer %d wins with %s!\n", winner_index, hand_rank_to_string(rank));
    printf("Won $%d\n", game->pot);

    wait_for_key_press();  // Ожидаем нажатия клавиши
}
