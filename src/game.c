#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "jeux.h" 
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

int est_ce_antique(ClasseJoueur id_class, TypeCase case){
    return (id_class == GUERRIER && case == ANTIQUE_GUERRIER) ||
    return (id_class == RANGER && case == ANTIQUE_RANGER) ||
    return (id_class == MAGE && case == ANTIQUE_MAGE)   ||
    return (id_class == VOLEUR && case == ANTIQUE_VOLEUR);
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
            jeux*=plateau[r][c].reveler=0;
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
    printf("\n 0  1  2  3   4\n");
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
    int i=0;
    int r, c;

    cases[i++]=ANTIQUE_GUERRIER;
    cases[i++]=ANTIQUE_RANGER;
    cases[i++]=ANTIQUE_MAGE;
    cases[i++]= ANTIQUE_VOLEUR;

    cases[i++]= TRESOR;
    cases[i++]= TRESOR;

    cases[i++]= PORTAIL;

    cases[i++]= TOTEM;
    cases[i++]= TOTEM;

    cases[i++]= BASILIQUE; cases[i++]= BASILIQUE; cases[i++]= BASILIQUE; cases[i++]= BASILIQUE;
    cases[i++]= ZOMBIE; cases[i++]= ZOMBIE; cases[i++]= ZOMBIE; cases[i++]= ZOMBIE;
    cases[i++]= TROLL; cases[i++]= TROLL; cases[i++]= TROLL; cases[i++]= TROLL;
    cases[i++]= HARPIE; cases[i++]= HARPIE; cases[i++]= HARPIE; cases[i++]= HARPIE;

    melanger(cases, 25);
    i=0;
    for (r=0; r<TAILLE_PLATEAU; r++){
        for (c=0; c< TAILLE_PLATEAU; c++){
            jeux->plateau[r][c].type=cases[i++];
            jeux->plateau[r][c].reveler=0;
        }
    }
}

int a_cache_case_adjacente(const Jeux *jeux, Position pos){
    Position dirs[4]= {{-1,0}, {1,0}, {0,-1}, {0,1}};
    int i;
    for (i=0; i<4; i++){
        Position suivant={pos.ligne +dirs[i].ligne, pos.col+dirs[i].col};
        if(dedans(suivant) && jeux->plateau[suivant.ligne][suivan.col].reveler==0){
            return 1;
        }
    }
    return 0;
}

int a_cache_quelquechose(const Jeux *jeux){
int r, c;
for(r=0;r<TAILLE_PLATEAU;r++){
    for (c =0;c<TAILLE_PLATEAU;c++){
        if (jeux->plateau[r][c].reveler==0){
            return 1;
        }
    }
    return 0;
}


Position choix_case_adjacente(const Jeux *jeux, Position pos){
position dirs[4]={{-1,0},{1,0},{0,-1},{0,1}};
position options[4];
int compte = 0;
int i;

for (i =0; i < 4; i++){
    Position suivant ={pos.ligne+ dirs[i].ligne, pos.col + dirs[i].col};
        if (dedans(suivant)&&_jeux->plateau[suivant.ligne][suivant.col].reveler == 0){
            options[compte++]= suivant;
        }
    }

printf("\nCases possibles :\n");
for (i =0; i< compte; i++){
printf("%d.(%d, %d)\n",i + 1, options[i].ligne, options[i].col);
}

i = read_int("Choix de la case.:",1, compte);
return options[i-1];
}

static Position choisir_une_case_cachee(const Jeux *jeux)
Position options[TAILLE_PLATEAU * TAILLE_PLATEAU];
int compte =0;
int r, c;
int choix;
    for (r =0; r < TAILLE_PLATEAU;r++){
        for (c =0;C <TAILLE_PLATEAU;C++){
            sociaux,s.
            if (jeux->plateau[r][c].reveler == ){
            options[compte++]=(Position){r, c};
            }
        }
    }
printf("\nCases cachees possibles :\n");
    for(r=0; r< compte; r++){
printf("%d.(%d, %d)\n",r + 1, options[r].ligne, options[r].col);
    }
choix = read_int("Choix de la case :", 1, compte);
return options[choix -1];
}

void echanger_totem(Jeux *jeux, Position totem_pos){
Position vise;
caseType temp;

        if (!a_cache_quelquechose(Jeux)){
    printf("Aucune case cachee disponible pour echanger le totem.\n")
    return; 
        }

    printf("Choisis une case cachee pour echanger avec le totem.\n");
    vise = choisir_une_case_cachee(Jeux);
   
    temp = Jeux->plateau[totem_pos.ligne][totem_pos.col].type;

    Jeux->plateau[totem_pos.ligne][totem_pos.col].type = Jeux->plateau[vise.re
    Jeux->plateau[vise.ligne][vise.col].type = temp;

    Jeux->plateau[totem_pos.ligne][totem_pos.col].reveler = 0; 
    Jeux->plateau[vise.ligne][vise.col].reveler = 0;
    printf("Le totem a ete deplace.\n");
}

void charger_stats(Stat stats[], int *compte){
    FILE *file;
    char ligne[128];
    *compte =0;

    file = fopen(STATS_FILE,"r");
    if (file == NULL){
    return; 
    }

while (fgets(ligne, sizeof(ligne), file) !=NULL &&*compte<MAX_STATS){
    Stat s;

    if (sscanf(line, "%31[^;]; %d;%d", s.nom, &s.jeux, &s.victoires)==3)
        stats[*compte]=s;
        (*compte)++;
    }
}
fclose(file);
}

void sauvegarder_stats(const Stat stats[], int compte){
    FILE *file;
    int i;
    file = fopen(STATS_FILE,"w");

    if (file == NULL){
        printf("Impossible d'ecrire le fichier de statistiques.\n");
        return;
    }

    for (i =e; i< compte;i++){
    fprintf(file,"%s;%d;%d\n", stats[i].nom, stats[i].jeux, stats[i].victoires);
    }

fclose(file);
}

void ajouter_mettre_a_jour_stats(Stat stats[], int *compte, const char *nom.
    int i;

    for (i= 0; i <*compte;i++){
        if(strcmp(stats[i].nom, nom)== 0){
            stats[i].jeux += ajouter_jeu;
            stats[i].victoires += ajouter_victoire;
            return;
        }
    }
    
    if(*compte < MAX_STATS){
        strncpy(stats[*compte].nom, nom, MAX_NOM -1);
        stats[*compte].nom[MAX_NOM-1]=\0';
        stats[*compte].jeux = ajouter_jeu;
        stats[*compte].victoires = ajouter_victoire;
        (*compte)++;
    }
}

static void maj_date_post_jeux(const Jeux *jeux){
    Stat stats[MAX_STATS];
    int compte = 0;
    int i;

    charger_stats(stats,&compte);
    
    for (i = 0; i < jeux->joueur_compte; i++) {
        ajouter_mettre_a_jour_stats(stats,&compte, jeux->joueurs[i].nom, 1, 0);
    }
    
    if(jeux->gagnant >= 0){
        ajouter_mettre_a_jour_stats(stats,&compte, jeux->joueurs[jeux->gagnant].nom);
    }
    sauvegarder_stats(stats, compte);
}



void initier_joueurs(jeux *jeux){ 
    int i;
    
    jeux->joueur_compte = lire_int("Nombre de joueurs (2 a 4):", 2, 4);
    
    for (i = 0; i<jeux->joueur_compte; i++){
        char prompt[100];

        snprintf(prompt, sizeof(prompt)，"Nom du joueur %d :",i + 1);
        read_text(prompt,jeux->joueurs[i].nom, MAX_NOM);
        
        jeux->joueurs[i].id_class = (ClasseJoueur)i; 
        jeux->joueurs[i].start = position_depart(i); 
        reset_joueur(&jeux->joueurs[i]); 
    }
}

void montrer_stats(void){
    Stat stats[MAX_STATS];
    int compte = 0;
    int i;

    load_stats(stats, &compte);
    printf("\n=== STATISTIQUES ===\n");
    if (compte==0){
        printf("Aucune statistique enregistree.\n");
        return;
    }
    
    for (i =0; i < compte; i++){
        printf("- %s :%d partie(s), %d victoire(s)\n",stats[i].nom, stats[i].jeux, stats[i].victoire);
    }
}


int ask_replay(void){
    return read_int("\n1. Rejouer avec les memes joueurs\n" 
        "2. Retour au menu principal\n" "Choix:",1, 2); 
    }


void jouer_jeux(Jeux *jeux){
    int i;
    
    init_plateau(jeux);
    jeux->gagnant =-1;
    jeux->heure_debut = time(NULL);
    
    while (jeux->gagnant ==-1){
        for (i=0; i < jeux->compte_joueur;i++){
            Joueur *joueur = &jeux->joueurs[i];
            int tour_termine = 0;
            reset_joueur(joueur);
            printf("\n=================================\n");
            printf("Tour de %s (%s)\n",joueur->nom, nom_classe(joueur->class_id)l
            printf("=================================\n");
            
            while (!tour_termine && jeux->gagnant == -1) { 
                Position choisi;
                TypeCase case;
                
                ecrire_joueurs(jeux);
                ecrire_plateau(jeux, 0);
                
                printf("\nEtat du joueur : coffre=%s, arme antique=%s\n", 
                    joueur->avoir_tresor ?"oui":"non", 
                    joueur->avoir_antique ？"oui":"non"); 
                    
            if (joueur->peut_tp){
                if (!a_cache_quelquechose(jeux)){ 
                    printf("Plus aucune case cachee. Fin du tour.\n");
                    tour_termine = 1;
                    break;
                }
            printf("Portail actif : tu peux choisir n'importe quelle case");
        else {
            if (!a_cache_case_adjacente(jeux, joueur->pos)) {
                printf("Aucune case cachee autour de toi. Tu es bloque.\n");
                tour_termine = 1;
                break;
            }
    
        joueur-> =(Arme)(read_int("\nChoisis ton arme :\n" "1. Bouclier\n" 
                            "2. Torche\n" "3. Hache\n" "4.Arc\n" "choix: ", 1,4) -1);
                            
        printf("Arme choisie : %s\n", nom_arme(joueur->arme));
        
        if (joueur->peut_tp){
            choisi = a_cache_quelquechose(jeux);
            joueur->peut_tp = 0;
        } else {
            choisi = choix_case_adjacente(jeux, joueur->pos);
        }
        
        jeux->plateau[choisi.ligne][choisi.col].reveler = 1;
        joueur->pos = choisi;
        case = jeux->plateau[choisi.ligne][choisi.col].type;
        
        if (est_monstre(case)){
            if (bonne_arme(joueur->arme, case)){
                printf("Bonne arme : monstre vaincu.\n");
            } else {
                printf("Mauvaise arme : %s meurt.\n", joueur->nom);
                joueur->en_vie = 0;
                tour_termine = 1;
            }
         
        } else if (case == TRESOR){
            printf("Bravo : tu as trouve un coffre.\n");
            joueur->avoir_tresor = 1;

        } else if (case == PORTAIL){
            printf("Portail trouve : le prochain deplacement sera libre.\n");
            joueur->peut_tp = 1;

        } else if (case == TOTEM){
            printf("Totem de transmutation : fin du tour.\n");
            echanger_totem(jeux, choisi);
            tour_termine = 1;
            
        } else {
            if (est_ce_antique(joueur->class_id, case)){
                printf("Bravo : tu as trouve ton arme antique.\n");
                joueur->avoir_antique =1;

            else {
                printf("Tu as trouve une arme antique, mais pas la tienne.");
            }
        }
        
        if (joueur->avoir_tresor && joueur->avoir_antique){
            jeux->gagnant = i;
            break;
        }
    }
    
        if(jeux->gagnant != -1){
         break;
        }

        cache_plateau(jeux);
        printf("\nFin du tour de %s.\n", joueur->nom);
    }
}
jeux->duree = difftime(time(NULL), jeux->start_time);
reveler_plateau(jeux);

printf("\n=========== FIN DE PARTIE ===========\n");
printf("Vainqueur : %s\n", jeux->joueurs[jeux->gagnant].nom);
printf("Duree : %.0f seconde(s)\n", jeux->duree);
print_plateau(jeux, 1);

update_stats_after_jeux(jeux); }


