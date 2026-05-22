#include <stdio.h>
#include <string.h>

#include "stats.h"
#include "display.h"

/*
 * Charge les statistiques depuis le fichier de sauvegarde. 
 (compte=nb entrée charger) */
void charger_stats(Stats stats[], int *compte) {
    FILE *file;
    char ligne[128];

     /* Initialisation du nombre de statistiques */
    *compte = 0;

    /* Ouverture du fichier des statistiques en lecture */
    file = fopen(STATS_FILE, "r");

    if (file == NULL) {
        return;
    }

    /* Lecture ligne par ligne du fichier */
    while (fgets(ligne, sizeof(ligne), file) != NULL && *compte < STATS_MAX) {
        Stats s;

        if (sscanf(ligne, "%31[^;];%d;%d",
                   s.nom,
                   &s.parties,
                   &s.victoires) == 3) {

            /* Ajout des statistiques au tableau */
            stats[*compte] = s;
            (*compte)++;
        }
    }

    fclose(file);
}

 /* Sauvegarde les statistiques dans le fichier*/
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

/* Ajoute ou met à jour les statistiques d'un joueur */
void ajouter_mettre_a_jour_stats(Stats stats[],
                                 int *compte,
                                 const char *nom,
                                 int ajouter_partie,
                                 int ajouter_victoire) {
    int i;

     /* Recherche du joueur dans les statistiques existantes */
    for (i = 0; i < *compte; i++) {
        if (strcmp(stats[i].nom, nom) == 0) {
            stats[i].parties += ajouter_partie;
            stats[i].victoires += ajouter_victoire;
            return;
        }
    }

     /* Si le joueur n'existe pas encore et qu'il reste de la place */
    if (*compte < STATS_MAX) {
        strncpy(stats[*compte].nom, nom, TAILLE_NOM_MAX - 1);
        stats[*compte].nom[TAILLE_NOM_MAX - 1] = '\0';

        /* Initialisation des statistiques */
        stats[*compte].parties = ajouter_partie;
        stats[*compte].victoires = ajouter_victoire;
        (*compte)++;
    }
}

/*
 * Met à jour les statistiques après une partie (jeux : structure
  contenant les joueurs et le gagnant) */
void maj_date_post_jeux(const Jeux *jeux) {
    Stats stats[STATS_MAX];
    int compte = 0;
    int i;

    charger_stats(stats, &compte);

    /* Ajout d'une partie jouée pour chaque joueur */
    for (i = 0; i < jeux->nombre_joueur; i++) {
        ajouter_mettre_a_jour_stats(stats,
                                    &compte,
                                    jeux->joueurs[i].nom,
                                    1,
                                    0);
    }

    /* Ajout d'une victoire pour le gagnant (s'il existe) */
    if (jeux->gagnant >= 0) {
        ajouter_mettre_a_jour_stats(stats,
                                    &compte,
                                    jeux->joueurs[jeux->gagnant].nom,
                                    0,
                                    1);
    }

    /* Sauvegarde des statistiques mises à jour */
    sauvegarder_stats(stats, compte);
}

/*Affiche les statistiques à l'écran*/
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