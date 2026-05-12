#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "game.h" 
#include "utilis.h"
#define RESET       "\033[0m"
#define GRAS        "\033[1m"

#define ROUGE       "\033[31m"
#define VERT        "\033[32m"
#define JAUNE       "\033[33m"
#define BLEU        "\033[34m"
#define VIOLET      "\033[35m"
#define CYAN        "\033[36m"
#define BLANC       "\033[37m"

void ecrire_tour(const Joueur *joueur){
    printf("\n");
    printf(BLEU "===============================================\n" RESET);
    printf(GRAS CYAN "Tour de %s" RESET "(%s)\n"
           joueur->nom,
           nom_classe(joueur->id_class));
    printf(BLUE "===============================================\n"RESET); v
}

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

void melanger (TypeCase cases[], int taille){
    int i;
    for(i=size-1;i>0;i--){
        int j=rand() %(i+1);
        TypeCase temp=cases[i];
        cases[i]=cases[j];
        cases[j]=temp;
    }
}

void cache_plateau(Jeux *jeu){
    int r,c;
    for (r=0; r<TAILLE_PLATEAU; r++){
        for (c=0; c< TAILLE_PLATEAU; c++){
            game*=plateau[r][c].reveler=0;
        }
    }
}
void reveler_plateau(Jeux *jeu){
    int r,c;
    for (r=0; r<TAILLE_PLATEAU; r++){
        for (c=0; c< TAILLE_PLATEAU; c++){
            jeux*=plateau[r][c].reveler=1;
        }
    }
}

void ecrire_joueurs(const Jeux *jeux, int joueur_actuelle){
    int i;
    printf(GRAS "\nJoueurs :\n"RESET);
    for(i=0; i<jeu->compte_joueur; i++){
        if(i==joueur_actuelle){
        printf(VERT"> %s (%s)[ENTRAIN DE JOUEUR]\n" RESET, jeu->joueurs[i].nom, nom_classe(jeu->joueur[i].id_class));
        }else{
            printf("%s(%s)\n",
                    jeu->joueurs[i].nom,
                    nom_classe(jeu->joueur[i].id_class));
        }
    }
}
void ecrire_plateau(const Jeux *jeux,  int montreTout){
    int r,c;
    for (r=0; r<TAILLE_PLATEAU; r++){
        printf("%d ", r);
        for (c=0; c< TAILLE_PLATEAU; c++){
            if (montreTout || jeu->plateau[r][c].reveler){
                printf("[%s]", code_case(jeux->plateau[r][c].reveler));
            } else {
                printf("[##]");
            }
        }
        printf("\n");
    }
}

void initier_plateau(Jeux *jeux){
    TypeCase cases[25];
    
}

