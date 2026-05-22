# Compilateur utilisé
CC = gcc

# Options de compilation :
# -Wall  : active la majorité des warnings
# -Wextra: warnings supplémentaires utiles
# -Werror: transforme les warnings en erreurs (code plus propre)
# -Iinclude : indique le dossier des headers

CFLAGS = -Wall -Wextra -Werror -Iinclude

# Liste des fichiers sources du projet
SRC = src/main.c src/jeu.c src/utilis.c src/display.c src/regles.c src/plateau.c src/stats.c src/configue_carte.c

# Transformation automatique des .c en .o (fichiers objets)
OBJ = $(SRC:.c=.o)

# Nom de l'exécutable final
TARGET = memo_rpg

# Règle par défaut : compile tout le projet
all: $(TARGET)

# Création de l'exécutable à partir des fichiers objets
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

# Nettoyage des fichiers générés (.o et exécutable)
clean:
	rm -f $(OBJ) $(TARGET)