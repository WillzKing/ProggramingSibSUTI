CC = gcc
CFLAGS = -Wall -Wextra -std=c11
TARGET = poker_game.exe
SOURCES = main.c deck.c player.c game.c ui.c ai.c
HEADERS = deck.h player.h game.h ui.h ai.h
OBJS = $(SOURCES:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	del /Q *.o *.exe

.PHONY: all clean