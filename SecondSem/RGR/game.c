#include "game.h"
#include "ui.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

void initialize_game(Game* game) {
    initialize_deck(&game->deck);
    shuffle_deck(&game->deck);

    game->community_cards_count = 0;
    game->pot = 0;
    game->current_player = 0;
    game->dealer_position = (game->dealer_position + 1) % MAX_PLAYERS;
    game->state = PREFLOP;

    place_blinds(game->players);
    deal_hole_cards(game);
}

void deal_hole_cards(Game* game) {
    for (int i = 0; i < MAX_PLAYERS; i++) {
        if (game->players[i].money > 0) {
            game->players[i].hand[0] = draw_card(&game->deck);
            game->players[i].hand[1] = draw_card(&game->deck);
        }
    }
}

void deal_flop(Game* game) {
    // Burn a card
    draw_card(&game->deck);

    // Deal flop
    for (int i = 0; i < 3; i++) {
        game->community_cards[game->community_cards_count++] = draw_card(&game->deck);
    }
    game->state = FLOP;
}

void deal_turn(Game* game) {
    // Burn a card
    draw_card(&game->deck);

    // Deal turn
    game->community_cards[game->community_cards_count++] = draw_card(&game->deck);
    game->state = TURN;
}

void deal_river(Game* game) {
    // Burn a card
    draw_card(&game->deck);

    // Deal river
    game->community_cards[game->community_cards_count++] = draw_card(&game->deck);
    game->state = RIVER;
}

int is_round_complete(const Game* game) {
    int active_players = 0;
    int players_acted = 0;

    for (int i = 0; i < MAX_PLAYERS; i++) {
        if (game->players[i].status == PLAYING && game->players[i].money > 0) {
            active_players++;
            if (game->players[i].has_acted ||
                game->players[i].current_bet == get_current_max_bet(game)) {
                players_acted++;
            }
        }
    }

    return active_players == players_acted || active_players <= 1;
}

void advance_round(Game* game) {
    reset_player_bets(game->players);
    game->current_player = (game->dealer_position + 1) % MAX_PLAYERS;

    switch (game->state) {
    case PREFLOP: deal_flop(game); break;
    case FLOP: deal_turn(game); break;
    case TURN: deal_river(game); break;
    case RIVER:
        game->state = SHOWDOWN;
        determine_winner(game);
        break;
    case SHOWDOWN:
        // Rotate dealer and start new hand
        game->dealer_position = (game->dealer_position + 1) % MAX_PLAYERS;
        initialize_game(game);
        break;
    }
}

int is_game_over(const Game* game) {
    return game->players[0].money <= 0;
}

int get_current_max_bet(const Game* game) {
    int max_bet = 0;
    for (int i = 0; i < MAX_PLAYERS; i++) {
        if (game->players[i].current_bet > max_bet) {
            max_bet = game->players[i].current_bet;
        }
    }
    return max_bet;
}

int compare_cards(const void* a, const void* b) {
    Card* cardA = (Card*)a;
    Card* cardB = (Card*)b;
    return cardB->value - cardA->value;
}

// Проверка на флеш
int is_flush(Card cards[], int count) {
    if (count < 5) return 0;

    int suit_counts[4] = { 0 };
    for (int i = 0; i < count; i++) {
        suit_counts[cards[i].suit]++;
    }

    for (int i = 0; i < 4; i++) {
        if (suit_counts[i] >= 5) return 1;
    }
    return 0;
}

// Проверка на стрит
int is_straight(Card cards[], int count) {
    if (count < 5) return 0;

    // Убираем дубликаты
    int unique_values[15] = { 0 };
    for (int i = 0; i < count; i++) {
        unique_values[cards[i].value] = 1;
    }

    // Проверяем последовательность из 5 карт
    int consecutive = 0;
    for (int i = 14; i >= 2; i--) {
        if (unique_values[i]) {
            consecutive++;
            if (consecutive == 5) return 1;
        }
        else {
            consecutive = 0;
        }
    }

    // Специальный случай: A-2-3-4-5
    if (unique_values[14] && unique_values[2] && unique_values[3] &&
        unique_values[4] && unique_values[5]) {
        return 1;
    }

    return 0;
}

// Оценка лучшей руки
BestHand evaluate_best_hand(Card player_hand[2], Card community_cards[5], int community_count) {
    BestHand best = { 0 };
    Card all_cards[7];
    int total_cards = 2 + community_count;

    // Собираем все карты
    memcpy(all_cards, player_hand, 2 * sizeof(Card));
    memcpy(all_cards + 2, community_cards, community_count * sizeof(Card));

    // Сортируем по убыванию
    qsort(all_cards, total_cards, sizeof(Card), compare_cards);

    // Проверяем комбинации от старших к младшим
    if (is_straight(all_cards, total_cards) && is_flush(all_cards, total_cards)) {
        // Проверяем роял-флеш и стрит-флеш
        if (all_cards[0].value == 14 && all_cards[1].value == 13 &&
            all_cards[2].value == 12 && all_cards[3].value == 11 &&
            all_cards[4].value == 10) {
            best.rank = ROYAL_FLUSH;
        }
        else {
            best.rank = STRAIGHT_FLUSH;
        }
    }
    else {
        // Здесь должна быть проверка других комбинаций...
        // Для краткости оставим только основные проверки
        best.rank = HIGH_CARD; // Заглушка
    }

    // Возвращаем 5 лучших карт
    memcpy(best.cards, all_cards, 5 * sizeof(Card));
    return best;
}

void determine_winner(Game* game) {
    BestHand best_hands[MAX_PLAYERS] = { 0 };
    int active_players = 0;
    int winners[MAX_PLAYERS] = { 0 };
    int winner_count = 0;
    HandRank max_rank = HIGH_CARD;

    // Оцениваем руки всех активных игроков
    for (int i = 0; i < MAX_PLAYERS; i++) {
        if (game->players[i].status != FOLDED && game->players[i].money > 0) {
            best_hands[i] = evaluate_best_hand(game->players[i].hand,
                game->community_cards,
                game->community_cards_count);
            active_players++;
            if (best_hands[i].rank > max_rank) {
                max_rank = best_hands[i].rank;
            }
        }
    }

    // Находим всех победителей
    for (int i = 0; i < MAX_PLAYERS; i++) {
        if (game->players[i].status != FOLDED &&
            game->players[i].money > 0 &&
            best_hands[i].rank == max_rank) {
            winners[winner_count++] = i;
        }
    }

    // Делим банк
    int win_amount = game->pot / winner_count;
    for (int i = 0; i < winner_count; i++) {
        game->players[winners[i]].money += win_amount;
    }

    // Показываем результаты (добавлено!)
    if (winner_count > 0) {
        // Выводим информацию о главном победителе
        display_winner(game, winners[0], max_rank);
    }

    game->pot = 0;
}