#ifndef PLATEAU_H
#define PLATEAU_H

#include "jeu.h"

void initier_plateau(Jeux *jeux);
void cache_plateau(Jeux *jeux);
void reveler_plateau(Jeux *jeux);

int a_cache_case_adjacente(const Jeux *jeux, Position pos);
int a_cache_quelquechose(const Jeux *jeux);

Position choix_case_adjacente(const Jeux *jeux, Position pos);
Position choisir_une_case_cachee(const Jeux *jeux);

void echanger_totem(Jeux *jeux, Position totem_pos);

#endif