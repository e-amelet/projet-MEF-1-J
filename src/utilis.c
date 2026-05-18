#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utilis.h"

int read_int(const char *prompt, int min, int max) {
    char ligne[100];
    char *finptr;
    long valeur;

    while (1) {
        printf("%s", prompt);

        if (fgets(ligne, sizeof(ligne), stdin) == NULL) {
            printf("Erreur de lecture.\n");
            continue;
        }

        valeur = strtol(ligne, &finptr, 10);

        if (finptr == ligne) {
            printf("Entre un nombre valide.\n");
            continue;
        }

        while (*finptr == ' ' || *finptr == '\t') {
            finptr++;
        }

        if (*finptr != '\n' && *finptr != '\0') {
            printf("Entree invalide.\n");
            continue;
        }

        if (valeur < min || valeur > max) {
            printf("Entre une valeur entre %d et %d.\n", min, max);
            continue;
        }

        return (int)valeur;
    }
}

void read_text(const char *prompt, char *buffer, int size) {
    char ligne[256];

    while (1) {
        printf("%s", prompt);

        if (fgets(ligne, sizeof(ligne), stdin) == NULL) {
            printf("Erreur de lecture.\n");
            continue;
        }

        ligne[strcspn(ligne, "\n")] = '\0';

        if (ligne[0] == '\0') {
            printf("Le texte ne peut pas etre vide.\n");
            continue;
        }

        strncpy(buffer, ligne, size - 1);
        buffer[size - 1] = '\0';

        return;
    }
}
