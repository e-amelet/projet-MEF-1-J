#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <locale.h>
#include "jeu.h"
#include "utils.h"

/* Affiche le menu principal du jeu et récupère le choix de l'utilisateur.*/
int menu_principal(void) {
    return lire_entier(
        "\n=== MEMO-RPG ===\n"
        "1. Nouvelle partie\n"
        "2. Voir les statistiques\n"
        "3. Quitter\n"
        "choix : ",
        1,
        3
    );
}

/* Point d'entrée principal du programme*/
int main(){
     /* Activation de la locale système (accents, emojis, etc.) */
    setlocale(LC_ALL, "");
    int lancer = 1;

    /* Initialisation du générateur de nombres aléatoires */
    srand(time(NULL));

    /* Boucle principale du programme */
    while(lancer){
        int choix = menu_principal();

        /* Nouvelle partie */
        if(choix == 1){
            Jeux jeux;
            int replay;

            /* Initialisation des joueurs et configuration des cartes */
            init_joueurs(&jeux);

            /* Boucle de rejouabilité */
            do{
                jouer_jeux(&jeux);
                replay = demande_replay();
            }while(replay == 1);

            /* Affichage des statistiques */
        }else if(choix == 2){
            montrer_stats();

            /* Quitter le jeu */
        }else{
            printf("au revoir.\n");
            lancer = 0;
        }
    }

    return 0;
}


