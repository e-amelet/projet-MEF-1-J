#include <stdio.h>
#include <stdlib.h>
#include "configue_carte.h"
#include "plateau.h"
#include "regles.h"
#include "display.h"
#include "utils.h"

/* Mélange aléatoirement un tableau de TypeCase */
static void melanger(TypeCase cases[], int taille) {
    int i;

    for (i = taille - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        TypeCase temp = cases[i];

        cases[i] = cases[j];
        cases[j] = temp;
    }
}

/* Initialise le plateau de jeu */ 
void initier_plateau(Jeux *jeux) {
    TypeCase cases[TAILLE_PLATEAU * TAILLE_PLATEAU];
    int i = 0;
    int r;
    int c;
    int type;
    int k;

    /* Vérifie ou corrige la configuration des cartes */
    verifier_ou_corriger_value_base_cartes(jeux);

    /* Remplissage du tableau de cases selon les valeurs définies */
    for (type = BASILIC; type <= ANTIQUE_VOLEUR; type++) {
        for (k = 0; k < jeux->value_base_cartes[type]; k++) {
            cases[i++] = (TypeCase)type;
        }
    }

    /* Mélange aléatoire des cases */
    melanger(cases, TAILLE_PLATEAU * TAILLE_PLATEAU);

    i = 0;

    /* Placement des cases dans la grille du plateau */
    for (r = 0; r < TAILLE_PLATEAU; r++) {
        for (c = 0; c < TAILLE_PLATEAU; c++) {
            jeux->plateau[r][c].type = cases[i++];
            jeux->plateau[r][c].revele = 0;
        }
    }
}

/*Définit la visibilité de toutes les cases du plateau (0=cache ou 1=revele)*/
static void definir_visibilite_plateau(Jeux *jeux, int valeur) {
    int r;
    int c;

    for (r = 0; r < TAILLE_PLATEAU; r++) {
        for (c = 0; c < TAILLE_PLATEAU; c++) {
            jeux->plateau[r][c].revele = valeur;
        }
    }
}

void cache_plateau(Jeux *jeux) {
    definir_visibilite_plateau(jeux, 0);
}

void reveler_plateau(Jeux *jeux) {
    definir_visibilite_plateau(jeux, 1);
}

/*Collecte les cases adjacentes (haut, bas, gauche, droite) qui sont encore
 cachées et retourne le nombre de cases trouvées.*/
static int collecter_cases_adjacentes_cachees(const Jeux *jeux,
                                              Position pos,
                                              Position options[]) {
    Position dirs[4] = {
        {-1, 0},
        {1, 0},
        {0, -1},
        {0, 1}
    };

    int i;
    int compte = 0;

    for (i = 0; i < 4; i++) {
        Position suivant = {
            pos.ligne + dirs[i].ligne,
            pos.col + dirs[i].col
        };

        if (dedans(suivant) &&
            jeux->plateau[suivant.ligne][suivant.col].revele == 0) {
            options[compte++] = suivant;
        }
    }

    return compte;
}

/*Indique s’il existe au moins une case cachée adjacente à la position donnée.*/
int a_cache_case_adjacente(const Jeux *jeux, Position pos) {
    Position options[4];

    return collecter_cases_adjacentes_cachees(jeux, pos, options) > 0;
}

/* Collecte toutes les cases cachées du plateau. Retourne le nombre total 
de cases cachées.*/
static int collecter_cases_cachees(const Jeux *jeux, Position options[]) {
    int r;
    int c;
    int compte = 0;

    for (r = 0; r < TAILLE_PLATEAU; r++) {
        for (c = 0; c < TAILLE_PLATEAU; c++) {
            if (jeux->plateau[r][c].revele == 0) {
                options[compte++] = (Position){r, c};
            }
        }
    }

    return compte;
}

/*Indique s’il reste au moins une case cachéesur l’ensemble du plateau.*/
int a_cache_quelquechose(const Jeux *jeux) {
    Position options[TAILLE_PLATEAU * TAILLE_PLATEAU];

    return collecter_cases_cachees(jeux, options) > 0;
}

/* Permet au joueur de choisir une case adjacente cachée.
 Affiche les options possibles et retourne la position choisie. */
Position choix_case_adjacente(const Jeux *jeux, Position pos) {
    Position options[4];
    int compte;
    int i;
    int choix;

    compte = collecter_cases_adjacentes_cachees(jeux, pos, options);

    printf("\nCases possibles :\n");

    for (i = 0; i < compte; i++) {
        printf("%d. (%d, %d)\n",
               i + 1,
               options[i].ligne,
               options[i].col);
    }

    choix = lire_entier("Choix de la case : ", 1, compte);

    return options[choix - 1];
}

/* Permet de choisir n’importe quelle case cachée du plateau.
 */
Position choisir_une_case_cachee(const Jeux *jeux) {
    Position options[TAILLE_PLATEAU * TAILLE_PLATEAU];
    int compte;
    int i;
    int choix;

    compte = collecter_cases_cachees(jeux, options);

    printf("\nCases cachees possibles :\n");

    for (i = 0; i < compte; i++) {
        printf("%d. (%d, %d)\n",
               i + 1,
               options[i].ligne,
               options[i].col);
    }

    choix = lire_entier("Choix de la case : ", 1, compte);

    return options[choix - 1];
}

/*Effet du totem (echange de position avec une case cachée choisie
 et recache des deux cases après l’échange) */
void echanger_totem(Jeux *jeux, Position totem_pos) {
    Position vise;
    TypeCase temp;

    if (!a_cache_quelquechose(jeux)) {
        printf(JAUNE "Aucune case cachee disponible pour echanger le totem.\n" RESET);
        return;
    }

    printf(JAUNE "Choisis une case cachee pour echanger avec le totem.\n" RESET);

    vise = choisir_une_case_cachee(jeux);

    /* Échange des types de cases */
    temp = jeux->plateau[totem_pos.ligne][totem_pos.col].type;
    jeux->plateau[totem_pos.ligne][totem_pos.col].type =
        jeux->plateau[vise.ligne][vise.col].type;
    jeux->plateau[vise.ligne][vise.col].type = temp;

    /* Les deux cases sont recachées */
    jeux->plateau[totem_pos.ligne][totem_pos.col].revele = 0;
    jeux->plateau[vise.ligne][vise.col].revele = 0;

    printf(JAUNE "Le totem a ete deplace.\n" RESET);
}