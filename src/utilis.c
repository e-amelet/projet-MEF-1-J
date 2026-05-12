#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utilis.h"

int lire_entier(char *prompt, int min, int max){
    char ligne[100];
    char *finptr
    long valeur


while(1){
    printf("%s",prompt);

    if(fgets(ligne, sizeof(ligne),stdin)==NULL){
        printf("Erreur de lecture.\n");
        continue;
    }
    valeur= strol(ligne, &finptr, 10);

    if(finptr==ligne){
        printf("Entre un nombre valide.\n");
        continue
    }
    while(*finptr ==' ' || *finptr == '\t'){
        finptr++
    }
    if(*finptr != '\n' && *finptr !='\0'){
        printf("Entree invalide.\n");
        continue
    }
    if (valeur<min || valeur>max){
        printf("Entree une valeur entre %d e %d.\n", min, max);
        continue
    }
    return (int)value;
    }
} 

void lire_texte(char *prompt, char *buffer, int size){
    char ligne[256];

    while(1){
         printf("%s", prompt);

        if (fgets(line, sizeof(line), stdin) == NULL) {
            printf("Erreur de lecture.\n");
            continue;
        }

        line[strcspn(line, "\n")] = '\0';

        if (line[0] == '\0') {
            printf("Le texte ne peut pas etre vide.\n");
            continue;
        }

        strncpy(buffer, line, size - 1);
        buffer[size - 1] = '\0';
        return;
    }
    }
