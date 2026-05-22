#ifndef RULES_H
#define RULES_H

#include "jeu.h"

int dedans(Position p);
int est_monstre(TypeCase type);
int bonne_arme(Arme arme, TypeCase monstre);
int est_ce_antique(ClasseJoueur id_class, TypeCase type_case);

Position position_depart(int index);
void respawn_joueur(Joueur *joueur);

#endif