#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "game.h" 
#include "utilis.h"

char *nom_classe(ClasseJoueur id_class){
    switch(id_class){
    case GUERRIER: return "Guerrier";
    case RANGER: return "Ranger";
    case MAGE: return "Mage";
    case VOLEUR: return "Voleur";
        default:   return "Inconnue";  
    }
}

char *nom_arme(Arme arme){
    switch(arme){
    case BOUCLIER: return "Bouclier";
    case TORCHE:  return"torche";
    case HACHE:   return"Hache";
    case ARC:     return "Arc";
    default: return"Inconnu";
    }
}
char code_case(TypeCase type){
    switch(type){
    case BASILIQUE:             return "BAS";
    case ZOMBIE:                return "ZOMB" ;
    case TROLL:                 return "TROL";
    case HARPIE:                return "HARP";
    case TRESOR:                return "TRS";
    case PORTAIL:               return "PORT";
    case TOTEM:                 return "TOT";
    case ANTIQUE_GUERRIER:      return "A_G";
    case ANTIQUE_RANGER:        return "A_R";
    case ANTIQUE_MAGE:          return "A_M";
    case ANTIQUE_VOLEUR:        return "A_V";
    default:                    return "??";
    }
}

int dedans(Position p){
    return p.ligne >= 0 && p.ligne < TAILLE_PLATEAU && p.col>= 0 && p.col < TAILLE_PLATEAU;
}

int est_monstre(TypeCase type){
    return type == BASILIQUE || type == ZOMBIE || type == TROLL || type == HARPIE;
}

int bonne_arme(Arme arme, TypeCase monstre){
    return(arme == BOUCLIER && monstre == BASILIQUE)||
          (arme == TORCHE && monstre == ZOMBIE)     ||
          (arme == HACHE && monstre == TROLL)       ||
          (arme == ARC && monstre == HARPIE);        
}  

int est_ce_antique(ClasseJoueur id_class, TypeCase tile){
    return (id_class == GUERRIER && tile == ANTIQUE_GUERRIER) ||
    return (id_class == RANGER && tile == ANTIQUE_RANGER) ||
    return (id_class == MAGE && tile == ANTIQUE_MAGE)   ||
    return (id_class == VOLEUR && tile == ANTIQUE_VOLEUR);
}


void respawn_joueur(Joueur *joueur){
    joueur->pos = joueur->start;
    joueur->avoir_tresor = 0;
    joueur->avoir_antique = 0;
    joueur-> en_vie = 1;
    joueur-> peut_tp = 1;
    joueur->arme= BOUCLIER;   
}

void 