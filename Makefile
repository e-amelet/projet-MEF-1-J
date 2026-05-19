CC = gcc
CFLAGS = -Wall -Wextra -Werror -Iinclude
SRC = src/main.c src/game.c src/utilis.c src/display.c src/rules.c src/board.c src/stats.c
OBJ = $(SRC:.c=.o)
TARGET = memo_rpg

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

clean:
	rm -f $(OBJ) $(TARGET)