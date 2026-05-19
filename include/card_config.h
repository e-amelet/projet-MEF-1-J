#ifndef CARD_CONFIG_H
#define CARD_CONFIG_H

#include "game.h"

void initialiser_value_base_cartes(Jeux *jeux);
void configurer_value_base_cartes(Jeux *jeux);
void verifier_ou_corriger_value_base_cartes(Jeux *jeux);

int total_value_base_cartes(const Jeux *jeux);
void afficher_value_base_cartes(const Jeux *jeux);

#endif