#include <stdio.h>
#include <string.h>

#include "stats.h"
#include "display.h"

void charger_stats(Stats stats[], int *compte) {
    FILE *file;
    char ligne[128];

    *compte = 0;

    file = fopen(STATS_FILE, "r");

    if (file == NULL) {
        return;
    }

    while (fgets(ligne, sizeof(ligne), file) != NULL && *compte < STATS_MAX) {
        Stats s;

        if (sscanf(ligne, "%31[^;];%d;%d",
                   s.nom,
                   &s.parties,
                   &s.victoires) == 3) {
            stats[*compte] = s;
            (*compte)++;
        }
    }

    fclose(file);
}

void sauvegarder_stats(const Stats stats[], int compte) {
    FILE *file;
    int i;

    file = fopen(STATS_FILE, "w");

    if (file == NULL) {
        printf(ROUGE "Impossible d'ecrire le fichier de statistiques.\n" RESET);
        return;
    }

    for (i = 0; i < compte; i++) {
        fprintf(file, "%s;%d;%d\n",
                stats[i].nom,
                stats[i].parties,
                stats[i].victoires);
    }

    fclose(file);
}

void ajouter_mettre_a_jour_stats(Stats stats[],
                                 int *compte,
                                 const char *nom,
                                 int ajouter_partie,
                                 int ajouter_victoire) {
    int i;

    for (i = 0; i < *compte; i++) {
        if (strcmp(stats[i].nom, nom) == 0) {
            stats[i].parties += ajouter_partie;
            stats[i].victoires += ajouter_victoire;
            return;
        }
    }

    if (*compte < STATS_MAX) {
        strncpy(stats[*compte].nom, nom, TAILLE_NOM_MAX - 1);
        stats[*compte].nom[TAILLE_NOM_MAX - 1] = '\0';
        stats[*compte].parties = ajouter_partie;
        stats[*compte].victoires = ajouter_victoire;
        (*compte)++;
    }
}

void maj_date_post_jeux(const Jeux *jeux) {
    Stats stats[STATS_MAX];
    int compte = 0;
    int i;

    charger_stats(stats, &compte);

    for (i = 0; i < jeux->nombre_joueur; i++) {
        ajouter_mettre_a_jour_stats(stats,
                                    &compte,
                                    jeux->joueurs[i].nom,
                                    1,
                                    0);
    }

    if (jeux->gagnant >= 0) {
        ajouter_mettre_a_jour_stats(stats,
                                    &compte,
                                    jeux->joueurs[jeux->gagnant].nom,
                                    0,
                                    1);
    }

    sauvegarder_stats(stats, compte);
}

void montrer_stats(void) {
    Stats stats[STATS_MAX];
    int compte = 0;
    int i;

    charger_stats(stats, &compte);

    printf(GRAS "\n=== STATISTIQUES ===\n" RESET);

    if (compte == 0) {
        printf("Aucune statistique enregistree.\n");
        return;
    }

    for (i = 0; i < compte; i++) {
        printf("- %s : %d partie(s), %d victoire(s)\n",
               stats[i].nom,
               stats[i].parties,
               stats[i].victoires);
    }
}