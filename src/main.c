#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "game.h"
#include "utils.h"

int menu_principal(void) {
    return lire_entier(
        "\n=== MEMO-RPG ===\n"
        "1. Nouvelle partie\n"
        "2. Voir les statistiques\n"
        "3. Quitter\n"
        "choix : ",
        1, 3
    );
}

int main(){
    int lancer = 1
    ntm
    srand(time(NULL));
    while(lancer){
        int choix =menu_principal();
        if(choix==1){
            Jeux jeux;
            int rejouer;


            init_joueur(&jeux);
            do{
                jouer_jeux(&jeux)
                replay=demande_replay();
            }while(replay==1);
        }else if (choix==2){
            montre_stats();
        }else{
            printf("au revoir.\n");
            lancer = 0;
        }
    }
    return 0;
}
