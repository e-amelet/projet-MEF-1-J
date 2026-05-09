#ifndef GAME_H
#define GAME_H
#include <time.h>
#define TAILLE_PLATEAU 5
#define JOUEUR_MIN 2
#define JOUEUR_MAX 4
#define TAILLE_NOM_MAX 32
#define STATS_MAX 200
#define STATS_FILE "Stats_joueurs.txt"

typedef enum{
    Guerrier;
    Ranger;
    Mage;
    Voleur;
}ClassJoueur;

typedef enum{
    Bouclier;
    Torche;
    Hache;
    Arc;
}Arme;

typedef enum{
    Basilic;
    Zombie;
    Troll;
    Harpie;
    Trésor;
    Portail ;
    Totem;
    Antique_Guerrier;
    Antique_Ranger;
    Antique_Mage;
    Antique_Voleur;
} TypeCase;
typedef struct{
    int ligne
    int colonne      
}Position;

typedef struct {
    TypeCase type;
    int révélé;
}Case;

