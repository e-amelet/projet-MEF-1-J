#include "regles.h"

/*Vérifie si une position se trouve à l'intérieur des limites du plateau de jeu.
Retourne 1 si oui, 0 sinon.*/
int dedans(Position p) {
    return p.ligne >= 0 &&
           p.ligne < TAILLE_PLATEAU &&
           p.col >= 0 &&
           p.col < TAILLE_PLATEAU;
}


int est_monstre(TypeCase type) {
    return type == BASILIC ||
           type == ZOMBIE ||
           type == TROLL ||
           type == HARPIE;
}

/*Vérifie si l'arme choisie est efficace contre le monstre rencontré. */
int bonne_arme(Arme arme, TypeCase monstre) {
    return (arme == BOUCLIER && monstre == BASILIC) ||
           (arme == TORCHE   && monstre == ZOMBIE)  ||
           (arme == HACHE    && monstre == TROLL)   ||
           (arme == ARC      && monstre == HARPIE);
}

/*Vérifie si une arme antique correspond à la classe du joueur.*/
int est_ce_antique(ClasseJoueur id_class, TypeCase type_case) {
    return (id_class == GUERRIER && type_case == ANTIQUE_GUERRIER) ||
           (id_class == RANGER   && type_case == ANTIQUE_RANGER)   ||
           (id_class == MAGE     && type_case == ANTIQUE_MAGE)     ||
           (id_class == VOLEUR   && type_case == ANTIQUE_VOLEUR);
}

/*Retourne la position de départ d’un joueur selon son index. Les positions
 sont placées à l'extérieur du plateau pour obliger une entrée.*/
Position position_depart(int index) {
    switch (index) {
        case 0:
            return (Position){-1, 2};
        case 1:
            return (Position){2, 5};
        case 2:
            return (Position){5, 2};
        case 3:
            return (Position){2, -1};
        default:
            return (Position){-1, 2};
    }
}

/*Réinitialise l'état d'un joueur au début d'une partie ou d'un tour. */
void respawn_joueur(Joueur *joueur) {
    joueur->pos = joueur->depart;
    joueur->a_un_tresor = 0;
    joueur->a_un_antique = 0;
    joueur->en_vie = 1;
    joueur->peut_tp = 0;
    joueur->arme = BOUCLIER;
}