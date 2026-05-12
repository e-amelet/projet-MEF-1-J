CC = gcc
CFLAGS = -Wall -Wextra -Werror -Iinclude
SRC = src/main.c scr/game.c src/utilis.C
OBJ = $(SRC:.c=.o)
TARGET = memo_rpg
all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

clean:
rm _f $(OBJ) $(TARGET)