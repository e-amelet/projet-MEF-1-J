#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"

/* Lit un entier saisi par l'utilisateur avec validation complète
  (l'entrée est un nombre, pas de caractères invalides,dans l'intervalle [min,max])*/
int lire_entier(const char *prompt, int min, int max) {
    char ligne[100];
    char *finptr;
    long valeur;

    while (1) {
        printf("%s", prompt);

        if (fgets(ligne, sizeof(ligne), stdin) == NULL) {
            printf("Erreur de lecture.\n");
            continue;
        }

         /* Conversion de la chaîne en entier long */
        valeur = strtol(ligne, &finptr, 10);

         /* Vérifie qu'au moins un nombre a été lu */
        if (finptr == ligne) {
            printf("Entre un nombre valide.\n");
            continue;
        }

        /* Ignore les espaces après le nombre */
        while (*finptr == ' ' || *finptr == '\t') {
            finptr++;
        }

        /* Vérifie qu'il ne reste que \n ou fin de chaîne */
        if (*finptr != '\n' && *finptr != '\0') {
            printf("Entree invalide.\n");
            continue;
        }

         /* Vérifie les bornes */
        if (valeur < min || valeur > max) {
            printf("Entre une valeur entre %d et %d.\n", min, max);
            continue;
        }

        return (int)valeur;
    }
}

/*Lit une chaîne de caractères saisie par l'utilisateur */
void read_text(const char *prompt, char *buffer, int size) {
    char ligne[256];

    while (1) {
        printf("%s", prompt);

        /* Lecture de la ligne */
        if (fgets(ligne, sizeof(ligne), stdin) == NULL) {
            printf("Erreur de lecture.\n");
            continue;
        }

        /* Suppression du \n final */
        ligne[strcspn(ligne, "\n")] = '\0';

         /* Vérifie que l'utilisateur n'a pas entré une chaîne vide */
        if (ligne[0] == '\0') {
            printf("Le texte ne peut pas etre vide.\n");
            continue;
        }

        /* Copie sécurisée dans le buffer de sortie */
        strncpy(buffer, ligne, size - 1);
        buffer[size - 1] = '\0';

        return;
    }
}
