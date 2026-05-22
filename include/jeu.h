#ifndef JEU_H
#define JEU_H
#include <time.h>

#define TAILLE_PLATEAU 5
#define JOUEUR_MIN 2
#define JOUEUR_MAX 4
#define TAILLE_NOM_MAX 32
#define STATS_MAX 200
#define STATS_FILE "Stats_joueurs.txt"

/*les différentes structures*/

typedef enum {
    GUERRIER,
    RANGER,
    MAGE,
    VOLEUR
} ClasseJoueur;

typedef enum {
    BOUCLIER,
    TORCHE,
    HACHE,
    ARC
} Arme;

typedef enum {
    BASILIC,
    ZOMBIE,
    TROLL,
    HARPIE,
    TRESOR,
    PORTAIL,
    TOTEM,
    ANTIQUE_GUERRIER,
    ANTIQUE_RANGER,
    ANTIQUE_MAGE,
    ANTIQUE_VOLEUR,
    NB_TYPE_CASES    /*permet d'avoir le nombre exact de type de case différentes dans les tableaux*/
} TypeCase;

typedef struct{
   int ligne;       
    int col;
}Position;

typedef struct {
    TypeCase type; /*chaque case a un type, peut etre reveler ou non*/
    int revele;
}Case;

typedef struct {
    char nom[TAILLE_NOM_MAX];
    ClasseJoueur id_class;
    Position depart;
    Position pos;
    int a_un_tresor;        
    int a_un_antique;  /*Antique = arme antique*/
    int en_vie;         
    int peut_tp;
    Arme arme;
}Joueur;

typedef struct{
    char nom[TAILLE_NOM_MAX];
    int parties;
    int victoires;
}Stats;

typedef struct{
    Case plateau[TAILLE_PLATEAU][TAILLE_PLATEAU];
    Joueur joueurs[JOUEUR_MAX];
    int value_base_cartes[NB_TYPE_CASES];
    int nombre_joueur;
    int gagnant;
    time_t heure_depart;  /*time_t permet directement d'avoir le temps enregistrer en faisant la diff entre heure_depart et la fin grace a difftime*/
    double duree;            /*car difftime renvoie un double*/   
}Jeux;

void init_joueurs(Jeux *jeux);
void jouer_jeux(Jeux *jeux);
void montrer_stats();
int demande_replay();

#endif