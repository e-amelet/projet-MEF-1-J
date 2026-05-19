#ifndef DISPLAY_H
#define DISPLAY_H

#include "game.h"

#define RESET       "\033[0m"
#define GRAS        "\033[1m"

#define ROUGE       "\033[31m"
#define VERT        "\033[32m"
#define JAUNE       "\033[33m"
#define BLEU        "\033[34m"
#define VIOLET      "\033[35m"
#define CYAN        "\033[36m"
#define BLANC       "\033[37m"

const char *nom_classe(ClasseJoueur id_class);
const char *emoji_classe(ClasseJoueur id_class);

const char *nom_arme(Arme arme);
const char *emoji_arme(Arme arme);

const char *code_case(TypeCase type);
const char *couleur_case(TypeCase type);
const char *nom_type_case(TypeCase type);

void ecrire_tour(const Joueur *joueur);
void ecrire_joueurs(const Jeux *jeux, int joueur_actuel);
void ecrire_plateau(const Jeux *jeux, const Joueur *joueur, int tout_montrer);
void ecrire_legende(void);

#endif