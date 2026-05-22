CC = gcc
CFLAGS = -Wall -Wextra -Werror -Iinclude
SRC = src/main.c src/jeu.c src/utilis.c src/display.c src/regles.c src/plateau.c src/stats.c src/configue_carte.c
OBJ = $(SRC:.c=.o)
TARGET = memo_rpg

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

clean:
	rm -f $(OBJ) $(TARGET)