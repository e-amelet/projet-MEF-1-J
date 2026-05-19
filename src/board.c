#include <stdio.h>
#include <stdlib.h>
#include "card_config.h"
#include "board.h"
#include "rules.h"
#include "display.h"
#include "utils.h"

static void melanger(TypeCase cases[], int taille) {
    int i;

    for (i = taille - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        TypeCase temp = cases[i];

        cases[i] = cases[j];
        cases[j] = temp;
    }
}


void initier_plateau(Jeux *jeux) {
    TypeCase cases[TAILLE_PLATEAU * TAILLE_PLATEAU];
    int i = 0;
    int r;
    int c;
    int type;
    int k;

    verifier_ou_corriger_value_base_cartes(jeux);

    for (type = BASILIC; type <= ANTIQUE_VOLEUR; type++) {
        for (k = 0; k < jeux->value_base_cartes[type]; k++) {
            cases[i++] = (TypeCase)type;
        }
    }

    melanger(cases, TAILLE_PLATEAU * TAILLE_PLATEAU);

    i = 0;

    for (r = 0; r < TAILLE_PLATEAU; r++) {
        for (c = 0; c < TAILLE_PLATEAU; c++) {
            jeux->plateau[r][c].type = cases[i++];
            jeux->plateau[r][c].revele = 0;
        }
    }
}

void cache_plateau(Jeux *jeux) {
    int r;
    int c;

    for (r = 0; r < TAILLE_PLATEAU; r++) {
        for (c = 0; c < TAILLE_PLATEAU; c++) {
            jeux->plateau[r][c].revele = 0;
        }
    }
}

void reveler_plateau(Jeux *jeux) {
    int r;
    int c;

    for (r = 0; r < TAILLE_PLATEAU; r++) {
        for (c = 0; c < TAILLE_PLATEAU; c++) {
            jeux->plateau[r][c].revele = 1;
        }
    }
}

int a_cache_case_adjacente(const Jeux *jeux, Position pos) {
    Position dirs[4] = {
        {-1, 0},
        {1, 0},
        {0, -1},
        {0, 1}
    };

    int i;

    for (i = 0; i < 4; i++) {
        Position suivant = {
            pos.ligne + dirs[i].ligne,
            pos.col + dirs[i].col
        };

        if (dedans(suivant) &&
            jeux->plateau[suivant.ligne][suivant.col].revele == 0) {
            return 1;
        }
    }

    return 0;
}

int a_cache_quelquechose(const Jeux *jeux) {
    int r;
    int c;

    for (r = 0; r < TAILLE_PLATEAU; r++) {
        for (c = 0; c < TAILLE_PLATEAU; c++) {
            if (jeux->plateau[r][c].revele == 0) {
                return 1;
            }
        }
    }

    return 0;
}

Position choix_case_adjacente(const Jeux *jeux, Position pos) {
    Position dirs[4] = {
        {-1, 0},
        {1, 0},
        {0, -1},
        {0, 1}
    };

    Position options[4];
    int compte = 0;
    int i;
    int choix;

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

    printf("\nCases possibles :\n");

    for (i = 0; i < compte; i++) {
        printf("%d. (%d, %d)\n",
               i + 1,
               options[i].ligne,
               options[i].col);
    }

    choix = read_int("Choix de la case : ", 1, compte);

    return options[choix - 1];
}

Position choisir_une_case_cachee(const Jeux *jeux) {
    Position options[TAILLE_PLATEAU * TAILLE_PLATEAU];
    int compte = 0;
    int r;
    int c;
    int choix;

    for (r = 0; r < TAILLE_PLATEAU; r++) {
        for (c = 0; c < TAILLE_PLATEAU; c++) {
            if (jeux->plateau[r][c].revele == 0) {
                options[compte++] = (Position){r, c};
            }
        }
    }

    printf("\nCases cachees possibles :\n");

    for (r = 0; r < compte; r++) {
        printf("%d. (%d, %d)\n",
               r + 1,
               options[r].ligne,
               options[r].col);
    }

    choix = read_int("Choix de la case : ", 1, compte);

    return options[choix - 1];
}

void echanger_totem(Jeux *jeux, Position totem_pos) {
    Position vise;
    TypeCase temp;

    if (!a_cache_quelquechose(jeux)) {
        printf(JAUNE "Aucune case cachee disponible pour echanger le totem.\n" RESET);
        return;
    }

    printf(JAUNE "Choisis une case cachee pour echanger avec le totem.\n" RESET);

    vise = choisir_une_case_cachee(jeux);

    temp = jeux->plateau[totem_pos.ligne][totem_pos.col].type;
    jeux->plateau[totem_pos.ligne][totem_pos.col].type =
        jeux->plateau[vise.ligne][vise.col].type;
    jeux->plateau[vise.ligne][vise.col].type = temp;

    jeux->plateau[totem_pos.ligne][totem_pos.col].revele = 0;
    jeux->plateau[vise.ligne][vise.col].revele = 0;

    printf(JAUNE "Le totem a ete deplace.\n" RESET);
}