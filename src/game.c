#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "game.h"
#include "utils.h"
#define RESET       "\033[0m"
#define GRAS        "\033[1m"

#define ROUGE       "\033[31m"
#define VERT        "\033[32m"
#define JAUNE       "\033[33m"
#define BLEU        "\033[34m"
#define VIOLET      "\033[35m"
#define CYAN        "\033[36m"
#define BLANC       "\033[37m"


void ecrire_tour(const Joueur *joueur) {
    printf("\n");
    printf(BLEU "===============================================\n" RESET);
    printf(GRAS CYAN "Tour de %s %s" RESET " (%s)\n",
           emoji_classe(joueur->id_class),
           joueur->nom,
           nom_classe(joueur->id_class));
    printf(BLEU "===============================================\n" RESET);
}

char *nom_classe(ClasseJoueur class_id){
    switch(class_id){
    case GUERRIER: return "Guerrier";
    case RANGER: return "Ranger";
    case MAGE: return "Mage";
    case VOLEUR: return "Voleur";
        default:   return "Inconnue";  
        }
    }


const char *emoji_classe(ClasseJoueur id_class) {
    static const char *emojis[] = {
        "🛡️",  
        "🌿",  
        "🧙",  
        "🥷"   
    };

    if (id_class < GUERRIER || id_class > VOLEUR) {
        return "❔";
    }

    return emojis[id_class];
}

char *nom_arme(Arme arme){
    switch(arme){
    case BOUCLIER: return "Bouclier";
    case TORCHE:  return"torche";
    case HACHE:   return"Hache";
    case ARC:     return "Arc";
    default: return"Inconnu";
    }

const char *emoji_arme(Arme arme) {
    static const char *emojis[] = {
        "🛡️",  
        "🔦",  
        "🪓",  
        "🏹"   
    };

    if (arme < BOUCLIER || arme > ARC) {
        return "❔";
    }

    return emojis[arme];
}

}
const char *code_case(TypeCase type) {
    static const char *emojis[] = {
        "🐍",  
        "🧟", 
        "👹",  
        "🦅",  
        "💰",  
        "🌀",  
        "🗿",  
        "🔥",  
        "🪄",  
        "📖",  
        "🗡️"   
    };

    if (type < BASILIC || type > ANTIQUE_VOLEUR) {
        return "❔";
    }

    return emojis[type];

const char *couleur_case(TypeCase type) {
    if (type == TRESOR) {
        return VERT;
    }

    if (type == PORTAIL || type == TOTEM) {
        return JAUNE;
    }

    if (type == ANTIQUE_GUERRIER ||
        type == ANTIQUE_RANGER ||
        type == ANTIQUE_MAGE ||
        type == ANTIQUE_VOLEUR) {
        return VIOLET;
    }

    if (type == BASILIC ||
        type == ZOMBIE ||
        type == TROLL ||
        type == HARPIE) {
        return ROUGE;
    }

    return BLANC;
}
}
int dedans(Position p){
    return p.ligne >= 0 && p.ligne < TAILLE_PLATEAU && p.col>= 0 && p.col < TAILLE_PLATEAU;
}

int est_monstre(TypeCase type){
    return type == BASILIC || type == ZOMBIE || type == TROLL || type == HARPIE;
}

int bonne_arme(Arme arme, TypeCase monstre){
    return(arme == BOUCLIER && monstre == BASILIC)||
          (arme == TORCHE && monstre == ZOMBIE)     ||
          (arme == HACHE && monstre == TROLL)       ||
          (arme == ARC && monstre == HARPIE);        
}  

int est_ce_antique(ClasseJoueur id_classe, TypeCase type_case){
    return  (class_id == GUERRIER && type_case == ANTIQUE_GUERRIER) ||
            (class_id == RANGER && type_case == ANTIQUE_RANGER) ||
            (class_id == MAGE && type_case == ANTIQUE_MAGE)   ||
            (class_id == VOLEUR && type_case == ANTIQUE_VOLEUR);
}


void respawn_joueur(Joueur *joueur){
    joueur->pos = joueur->depart;
    joueur->a_un_tresor = 0;
    joueur->a_un_antique = 0;
    joueur->en_vie = 1;
    joueur-> peut_tp = 1;
    joueur->arme= BOUCLIER;   
}

void melanger (TypeCase cases[], int taille){
    int i;
    for(i=taille-1;i>0;i--){
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
            jeux->plateau[r][c].revele= 0;
        }
    }
}
void revele_plateau(Jeux *jeu){
    int r,c;
    for (r=0; r<TAILLE_PLATEAU; r++){
        for (c=0; c< TAILLE_PLATEAU; c++){
            jeux->plateau[r][c].revele= 1;
        }
    }
}

void ecrire_joueurs(const Jeux *jeux, int joueur_actuelle) {
    int i;
    printf(GRAS "\nJoueurs :\n" RESET);

    for (i = 0; i < jeux->nombre_joueur; i++) {
        if (i == joueur_actuelle) {
            printf(VERT "> %s %s (%s) [EN TRAIN DE JOUER]\n" RESET,
                   emoji_classe(jeux->joueurs[i].id_class),
                   jeux->joueurs[i].nom,
                   nom_classe(jeux->joueurs[i].id_class));
        } else {
            printf("  %s %s (%s)\n",
                   emoji_classe(jeux->joueurs[i].id_class),
                   jeux->joueurs[i].nom,
                   nom_classe(jeux->joueurs[i].id_class));
        }
    }
}

void ecrire_plateau(const Jeux *jeux, const Joueur *joueur, int tout_montrer) {
    int r;
    int c;

    printf(GRAS "\nPlateau :\n" RESET);
    printf("     0    1    2    3    4\n");

    for (r = 0; r < TAILLE_PLATEAU; r++) {
        printf("%d  ", r);

        for (c = 0; c < TAILLE_PLATEAU; c++) {
            if (joueur->pos.ligne == r && joueur->pos.col == c) {
                printf(CYAN " 🧍 " RESET);
            } else if (tout_montrer || jeux->plateau[r][c].revele) {
                TypeCase type = jeux->plateau[r][c].type;
                printf("%s %s " RESET, couleur_case(type), code_case(type));
            } else {
                printf(BLANC " ❓ " RESET);
            }
        }

        printf("\n");
    }
}

void ecrire_legende(void) {
    printf("\nLegende :\n");
    printf("❓ cachee | 🧍 joueur | 🐍 basilic | 🧟 zombie | 👹 troll | 🦅 harpie\n");
    printf("💰 tresor | 🌀 portail | 🗿 totem | 🔥 guerrier | 🪄 ranger | 📖 mage | 🗡️ voleur\n");
    printf("🛡️ bouclier | 🔦 torche | 🪓 hache | 🏹 arc\n");
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

    cases[i++]= BASILIC; cases[i++]= BASILIC; cases[i++]= BASILIC; cases[i++]= BASILIC;
    cases[i++]= ZOMBIE; cases[i++]= ZOMBIE; cases[i++]= ZOMBIE; cases[i++]= ZOMBIE;
    cases[i++]= TROLL; cases[i++]= TROLL; cases[i++]= TROLL; cases[i++]= TROLL;
    cases[i++]= HARPIE; cases[i++]= HARPIE; cases[i++]= HARPIE; cases[i++]= HARPIE;

    melanger(cases, 25);
    i=0;
    for (r=0; r<TAILLE_PLATEAU; r++){
        for (c=0; c< TAILLE_PLATEAU; c++){
            jeux->plateau[r][c].type=cases[i++];
            jeux->plateau[r][c].revele=0;
        }
    }
}

int a_cache_case_adjacente(const Jeux *jeux, Position pos){
    Position dirs[4]= {{-1,0}, {1,0}, {0,-1}, {0,1}};
    int i;
    for (i=0; i<4; i++){
        Position suivant={pos.ligne +dirs[i].ligne, pos.col+dirs[i].col};
        if(dedans(suivant) && jeux->plateau[suivant.ligne][suivant.col].==0){
            return 1;
        }
    }
    return 0;
}

int a_cache_quelquechose(const Jeux *jeux){
int r, c;
for(r=0;r<TAILLE_PLATEAU;r++){
    for (c =0;c<TAILLE_PLATEAU;c++){
        if (jeux->plateau[r][c].revele==0){
            return 1;
        }
    }
    return 0;
}


Position choix_case_adjacente(const Jeux *jeux, Position pos){
Position dirs[4]={{-1,0},{1,0},{0,-1},{0,1}};
position options[4];
int compte = 0;
int i;

for (i =0; i < 4; i++){
    Position suivant ={pos.ligne+ dirs[i].ligne, pos.col + dirs[i].col};
        if (dedans(suivant)&&_jeux->plateau[suivant.ligne][suivant.col].revele == 0){
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


static Position choisir_une_case_cachee(const Jeux *jeux){
Position options[TAILLE_PLATEAU * TAILLE_PLATEAU];
int compte =0;
int r, c;
int choix;
    for (r =0; r < TAILLE_PLATEAU;r++){
        for (c =0;C <TAILLE_PLATEAU;C++){
            sociaux,s.
            if (jeux->plateau[r][c].revele == ){
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

    Jeux->plateau[totem_pos.ligne][totem_pos.col].revele = 0; 
    Jeux->plateau[vise.ligne][vise.col].revele = 0;
    printf("Le totem a ete deplace.\n");
}

void charger_stats(Stats stats[], int *compte){
    FILE *file;
    char ligne[128];
    *compte =0;

    file = fopen(STATS_FILE,"r");
    if (file == NULL){
    return; 
    }

while (fgets(ligne, tailleof(ligne), file) !=NULL &&*compte<STATS_MAX){
    Stats s;

    if (sscanf(line, "%31[^;]; %d;%d", s.nom, &s.jeux, &s.victoires)==3)
        stats[*compte]=s;
        (*compte)++;
    }
}
fclose(file);
}

void sauvegarder_stats(const Stats stats[], int compte){
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

void ajouter_mettre_a_jour_stats(Stats stats[], int *compte, const char *nom.
    int i;

    for (i= 0; i <*compte;i++){
        if(strcmp(stats[i].nom, nom)== 0){
            stats[i].jeux += ajouter_jeu;
            stats[i].victoires += ajouter_victoire;
            return;
        }
    }
    
    if(*compte < STATS_MAX){
        strncpy(stats[*compte].nom, nom, MAX_NOM -1);
        stats[*compte].nom[MAX_NOM-1]=\0';
        stats[*compte].jeux = ajouter_jeu;
        stats[*compte].victoires = ajouter_victoire;
        (*compte)++;
    }
}

static void maj_date_post_jeux(const Jeux *jeux){
    Stats stats[STATS_MAX];
    int compte = 0;
    int i;

    charger_stats(stats,&compte);
    
    for (i = 0; i < jeux->nombre_joueur; i++) {
        ajouter_mettre_a_jour_stats(stats,&compte, jeux->joueurs[i].nom, 1, 0);
    }
    
    if(jeux->gagnant >= 0){
        ajouter_mettre_a_jour_stats(stats,&compte, jeux->joueurs[jeux->gagnant].nom);
    }
    sauvegarder_stats(stats, compte);
}



void init_joueurs(jeux *jeux){ 
    int i;
    
    jeux->nombre_joueur = lire_int("Nombre de joueurs (2 a 4):", 2, 4);
    
    for (i = 0; i<jeux->nombre_joueur; i++){
        char prompt[100];

        snprintf(prompt, tailleof(prompt)，"Nom du joueur %d :",i + 1);
        read_text(prompt,jeux->joueurs[i].nom, MAX_NOM);
        
        jeux->joueurs[i].class_id = (ClasseJoueur)i; 
        jeux->joueurs[i].start = position_depart(i); 
        reset_joueur(&jeux->joueurs[i]); 
    }
}

void voir_stats(){
    Stats stats[STATS_MAX];
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


int demande_replay(void)
{
    int choix;

    choix = lire_entier(
        "\n1. Rejouer avec les memes joueurs\n"
        "2. Retour au menu principal\n"
        "Choix : ",
        1,
        2
    );

    if (choix == 1) {
        return 1;
    }

    return 0;


void jouer_jeux(Jeux *jeux){
    int i;
    
    init_plateau(jeux);
    jeux->gagnant =-1;
    jeux->heure_depart = time(NULL);
    
    while (jeux->gagnant ==-1){
        for (i=0; i < jeux->nombre_joueur;i++){
            Joueur *joueur = &jeux->joueurs[i];
            int tour_termine = 0;
            reset_joueur(joueur);
            ecrire_tour(joueur);
            while (!tour_termine && jeux->gagnant == -1) { 
                Position choisi;
                TypeCase type_case;
                
                    ecrire_joueurs(jeux, i);
                    ecrire_plateau(jeux, joueur, 0);
                    ecrire_legende();
                
                printf("\nEtat du joueur : coffre=%s, arme antique=%s\n", 
                    joueur->a_un_tresor ?"oui":"non", 
                    joueur->a_un_antique ?"oui":"non"); 
                    
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
    
                joueur->arme = (Arme)(read_int(
        "\nChoisis ton arme :\n"
        "1. 🛡️ Bouclier\n"
        "2. 🔦 Torche\n"
        "3. 🪓 Hache\n"
        "4. 🏹 Arc\n"
        "Choix : ",
         1, 4
        ) - 1);
                            
       printf(CYAN "Arme choisie : %s %s\n" RESET,
       emoji_arme(joueur->arme),
       nom_arme(joueur->arme));
        
        if (joueur->peut_tp){
            choisi = a_cache_quelquechose(jeux);
            joueur->peut_tp = 0;
        } else {
            choisi = choix_case_adjacente(jeux, joueur->pos);
        }
        
        jeux->plateau[choisi.ligne][choisi.col].revele = 1;
        joueur->pos = choisi;
        type_case = jeux->plateau[choisi.ligne][choisi.col].type;
        printf(CYAN "\n%s se deplace vers (%d, %d).\n" RESET,
        joueur->nom,
        choisi.ligne,
        choisi.col);

        printf("Case revelee : %s%s%s\n",
        couleur_case(type_case),
        code_case(type_case),
        RESET);
        if (est_monstre(type_case)){
            if (bonne_arme(joueur->arme, type_case)){
                printf(VERT "Bonne arme : monstre vaincu.\n" RESET);
            } else {
               printf(ROUGE "Mauvaise arme : %s est vaincu.\n" RESET, joueur->nom);
                joueur->en_vie = 0;
                tour_termine = 1;
            }
         
        } else if (type_case == TRESOR){
           printf(VERT "Bravo : tu as trouve un coffre 💰.\n" RESET);
            joueur->a_un_tresor = 1;

        } else if (type_case == PORTAIL){
           printf(JAUNE "Portail trouve 🌀 : le prochain deplacement sera libre.\n" RESET);
            joueur->peut_tp = 1;

        } else if (type_case == TOTEM){
            printf(JAUNE "Totem de transmutation 🗿 : fin du tour.\n" RESET);
            echanger_totem(jeux, choisi);
            tour_termine = 1;
            
        } else {
            if (est_ce_antique(joueur->id_classe, type_case)){
                printf(VERT "Bravo : tu as trouve ton arme antique %s.\n" RESET,
                code_case(type_case));
                joueur->a_un_antique =1;

            else {
                printf("Tu as trouve une arme antique, mais pas la tienne.");
            }
        }
        
        if (joueur->a_un_tresor && joueur->a_un_antique){
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
jeux->duree = difftime(time(NULL), jeux->heure_depart);
revele_plateau(jeux);

printf(GRAS VERT "\n=========== FIN DE PARTIE ===========\n" RESET);
printf("Vainqueur : %s %s\n",
       emoji_classe(jeux->joueurs[jeux->gagnant].id_class),
       jeux->joueurs[jeux->gagnant].nom);
printf("Duree : %.0f seconde(s)\n", jeux->duree);

ecrire_plateau(jeux, &jeux->joueurs[jeux->gagnant], 1);
ecrire_legende();

update_stats_after_jeux(jeux); }



