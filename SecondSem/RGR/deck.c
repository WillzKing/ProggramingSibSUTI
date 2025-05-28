#include "deck.h"
#include <stdlib.h>
#include <time.h>
#include <string.h>

void initialize_deck(Deck* deck) {
    int index = 0;
    for (int suit = HEARTS; suit <= SPADES; suit++) {
        for (int value = 2; value <= 14; value++) {
            deck->cards[index].suit = (Suit)suit;
            deck->cards[index].value = value;
            index++;
        }
    }
    deck->top = 0;
}

void shuffle_deck(Deck* deck) {
    for (int i = 0; i < 52; i++) {
        int j = rand() % 52;
        Card temp = deck->cards[i];
        deck->cards[i] = deck->cards[j];
        deck->cards[j] = temp;
    }
    deck->top = 0;
}

Card draw_card(Deck* deck) {
    if (deck->top >= 52) {
        // Shouldn't happen in normal game, but handle it
        Card empty = { HEARTS, 0 };
        return empty;
    }
    return deck->cards[deck->top++];
}

const char* card_to_string(Card card) {
    static char str[20];
    const char* suit_str;
    const char* value_str;

    switch (card.suit) {
    case HEARTS: suit_str = "Hearts"; break;
    case DIAMONDS: suit_str = "Diamonds"; break;
    case CLUBS: suit_str = "Clubs"; break;
    case SPADES: suit_str = "Spades"; break;
    }

    switch (card.value) {
    case 11: value_str = "Jack"; break;
    case 12: value_str = "Queen"; break;
    case 13: value_str = "King"; break;
    case 14: value_str = "Ace"; break;
    default:
        sprintf(str, "%d", card.value);
        value_str = str;
        break;
    }

    sprintf(str, "%s of %s", value_str, suit_str);
    return str;
}