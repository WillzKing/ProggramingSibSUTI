#ifndef DECK_H
#define DECK_H

#include <stdio.h>

typedef enum {
    HEARTS,
    DIAMONDS,
    CLUBS,
    SPADES
} Suit;

typedef struct {
    Suit suit;
    int value; // 2-14 (11=Jack, 12=Queen, 13=King, 14=Ace)
} Card;

typedef struct {
    Card cards[52];
    int top;
} Deck;

void initialize_deck(Deck* deck);
void shuffle_deck(Deck* deck);
Card draw_card(Deck* deck);
const char* card_to_string(Card card);

#endif