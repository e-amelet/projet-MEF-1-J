#ifndef CONFIGUE_CARTE_H  /*espace pour le testeur de la valeur des cartes*/
#define CONFIGUE_CARTE_H  /*ifndef = permet le moins 'erreur lors de la compilation quand les fichier s'inclus entre eux.*/

#include "jeu.h"
/*initialisation des fonctions du testeurs.*/
void initialiser_value_base_cartes(Jeux *jeux);
void configurer_value_base_cartes(Jeux *jeux);
void verifier_ou_corriger_value_base_cartes(Jeux *jeux);

int total_value_base_cartes(const Jeux *jeux);
void afficher_value_base_cartes(const Jeux *jeux);

#endif