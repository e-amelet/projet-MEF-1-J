#ifndef CONFIGUE_CARTE_H  /*espace pour le testeur de la valeur des cartes*/
#define CONFIGUE_CARTE_H  /*ifndef = permet le moins 'erreur lors de la compilation quand les fichier s'inclus entre eux.*/

#include "jeu.h"
/*initialisation des fonctions du testeurs.*/
void initier_valeur_base_carte(Jeux *jeux);
void configurer_valeur_base_cartes(Jeux *jeux);
void verifier_ou_corriger_valeur_base_cartes(Jeux *jeux);

int total_valeur_base_cartes(const Jeux *jeux);
void afficher_valeur_base_cartes(const Jeux *jeux);

#endif