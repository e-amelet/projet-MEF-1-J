#ifndef STATS_H
#define STATS_H

#include "game.h"

void charger_stats(Stats stats[], int *compte);
void sauvegarder_stats(const Stats stats[], int compte);
void ajouter_mettre_a_jour_stats(Stats stats[],
                                 int *compte,
                                 const char *nom,
                                 int ajouter_partie,
                                 int ajouter_victoire);

void maj_date_post_jeux(const Jeux *jeux);
void montrer_stats(void);

#endif