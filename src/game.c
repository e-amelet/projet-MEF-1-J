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
#ifndef MAX_STATS
#define MAX_STATS 200
#endif

#ifndef MAX_NOM
#define MAX_NOM 32
#endif

const char *nom_classe(ClasseJoueur id_class);
const char *emoji_classe(ClasseJoueur id_class);
const char *nom_arme(Arme arme);
const char *emoji_arme(Arme arme);
const char *code_case(TypeCase type);
const char *couleur_case(TypeCase type);

Position choisir_une_case_cachee(const Jeux *jeux);

void ecrire_tour(const Joueur *joueur) {
    printf("\n");
    printf(BLEU "===============================================\n" RESET);
    printf(GRAS CYAN "Tour de %s %s" RESET " (%s)\n",
           emoji_classe(joueur->id_class),
           joueur->nom,
           nom_classe(joueur->id_class));
    printf(BLEU "===============================================\n" RESET);
}

const char *nom_classe(ClasseJoueur id_class) {
    switch (id_class) {
        case GUERRIER:
        return "Guerrier";
        case RANGER:
        return "Ranger";
        case MAGE:
        return "Mage";
        case VOLEUR:
        return "Voleur";
        default:
        return "Inconnue";
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

const char *nom_arme(Arme arme) {
    switch (arme) {
        case BOUCLIER:
        return "Bouclier";
        case TORCHE:
        return "Torche";
        case HACHE:
        return "Hache";
        case ARC:
        return "Arc";
        default:
        return "Inconnue";
    }
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
}

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

const char *nom_type_case(TypeCase type) {
    static const char *noms[] = {
        "Basilic",
        "Zombie",
        "Troll",
        "Harpie",
        "Tresor",
        "Portail",
        "Totem",
        "Antique guerrier",
        "Antique ranger",
        "Antique mage",
        "Antique voleur"
    };

    if (type < BASILIC || type > ANTIQUE_VOLEUR) {
        return "Inconnu";
    }

    return noms[type];
}

void initialiser_value_base_cartes(Jeux *jeux) {
    jeux->value_base_cartes[BASILIC] = 4;
    jeux->value_base_cartes[ZOMBIE] = 4;
    jeux->value_base_cartes[TROLL] = 3;
    jeux->value_base_cartes[HARPIE] = 4;

    jeux->value_base_cartes[TRESOR] = 2;
    jeux->value_base_cartes[PORTAIL] = 1;
    jeux->value_base_cartes[TOTEM] = 2;

    jeux->value_base_cartes[ANTIQUE_GUERRIER] = 1;
    jeux->value_base_cartes[ANTIQUE_RANGER] = 1;
    jeux->value_base_cartes[ANTIQUE_MAGE] = 1;
    jeux->value_base_cartes[ANTIQUE_VOLEUR] = 1;
}

int valeur_officielle_carte(TypeCase type) {
    static const int valeurs_officielles[] = {
        4,  /* BASILIC */
        4,  /* ZOMBIE */
        4,  /* TROLL */
        4,  /* HARPIE */
        2,  /* TRESOR */
        1,  /* PORTAIL */
        2,  /* TOTEM */
        1,  /* ANTIQUE_GUERRIER */
        1,  /* ANTIQUE_RANGER */
        1,  /* ANTIQUE_MAGE */
        1   /* ANTIQUE_VOLEUR */
    };

    if (type < BASILIC || type > ANTIQUE_VOLEUR) {
        return 0;
    }

    return valeurs_officielles[type];
}

int total_value_base_cartes(const Jeux *jeux) {
    int total = 0;
    int i;

    for (i = BASILIC; i <= ANTIQUE_VOLEUR; i++) {
        total += jeux->value_base_cartes[i];
    }

    return total;
}

int value_base_cartes_est_valide(const Jeux *jeux) {
    int i;
    int total = total_value_base_cartes(jeux);
    int valide = 1;

    if (total < TAILLE_PLATEAU * TAILLE_PLATEAU) {
        printf(ROUGE "\nErreur : il n'y a pas assez de cartes sur le terrain.\n" RESET);
        printf("Total actuel : %d / %d\n", total, TAILLE_PLATEAU * TAILLE_PLATEAU);
        valide = 0;
    } else if (total > TAILLE_PLATEAU * TAILLE_PLATEAU) {
        printf(ROUGE "\nErreur : il y a trop de cartes sur le terrain.\n" RESET);
        printf("Total actuel : %d / %d\n", total, TAILLE_PLATEAU * TAILLE_PLATEAU);
        valide = 0;
    }

    for (i = BASILIC; i <= ANTIQUE_VOLEUR; i++) {
        int attendu = valeur_officielle_carte((TypeCase)i);
        int actuel = jeux->value_base_cartes[i];

        if (actuel != attendu) {
            printf(ROUGE "Erreur : %s doit valoir %d, mais vaut %d.\n" RESET,
                   nom_type_case((TypeCase)i),
                   attendu,
                   actuel);
            valide = 0;
        }
    }

    return valide;
}

void afficher_value_base_cartes(const Jeux *jeux) {
    int i;

    printf(GRAS "\nConfiguration actuelle des cartes :\n" RESET);

    for (i = BASILIC; i <= ANTIQUE_VOLEUR; i++) {
        printf("- %s %s : %d\n",
               code_case((TypeCase)i),
               nom_type_case((TypeCase)i),
               jeux->value_base_cartes[i]);
    }

    printf("Total : %d / %d\n",
           total_value_base_cartes(jeux),
           TAILLE_PLATEAU * TAILLE_PLATEAU);
}

void saisir_value_base_cartes(Jeux *jeux) {
    int i;
    char prompt[120];

    printf(JAUNE "\nModification du nombre de cartes.\n" RESET);

    for (i = BASILIC; i <= ANTIQUE_VOLEUR; i++) {
        snprintf(prompt,
                 sizeof(prompt),
                 "Nombre de cartes pour %s %s : ",
                 code_case((TypeCase)i),
                 nom_type_case((TypeCase)i));

        jeux->value_base_cartes[i] = read_int(prompt, 0, 25);
    }
}

void verifier_ou_corriger_value_base_cartes(Jeux *jeux) {
    int choix;

    while (!value_base_cartes_est_valide(jeux)) {
        printf(JAUNE "\nLa configuration des cartes n'est pas valide.\n" RESET);
        afficher_value_base_cartes(jeux);

        choix = read_int(
            "\nQue veux-tu faire ?\n"
            "1. Corriger automatiquement avec les valeurs officielles\n"
            "2. Ressaisir les valeurs manuellement\n"
            "Choix : ",
            1,
            2
        );

        if (choix == 1) {
            initialiser_value_base_cartes(jeux);
            printf(VERT "Les valeurs officielles ont ete restaurees.\n" RESET);
        } else {
            saisir_value_base_cartes(jeux);
        }
    }
}

void configurer_value_base_cartes(Jeux *jeux) {
    int choix;

    initialiser_value_base_cartes(jeux);

    choix = read_int(
        "\nConfiguration des cartes :\n"
        "1. Garder les valeurs officielles\n"
        "2. Modifier les valeurs manuellement\n"
        "Choix : ",
        1,
        2
    );

    if (choix == 2) {
        saisir_value_base_cartes(jeux);
        verifier_ou_corriger_value_base_cartes(jeux);
    }

    afficher_value_base_cartes(jeux);
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

int est_ce_antique(ClasseJoueur id_class, TypeCase type_case){
    return  (id_class == GUERRIER && type_case == ANTIQUE_GUERRIER) ||
            (id_class == RANGER && type_case == ANTIQUE_RANGER) ||
            (id_class == MAGE && type_case == ANTIQUE_MAGE)   ||
            (id_class == VOLEUR && type_case == ANTIQUE_VOLEUR);
}


void respawn_joueur(Joueur *joueur){
    joueur->pos = joueur->depart;
    joueur->a_un_tresor = 0;
    joueur->a_un_antique = 0;
    joueur->en_vie = 1;
    joueur-> peut_tp = 0;
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

void cache_plateau(Jeux *jeu) {
    int r;
    int c;

    for (r = 0; r < TAILLE_PLATEAU; r++) {
        for (c = 0; c < TAILLE_PLATEAU; c++) {
            jeu->plateau[r][c].revele = 0;
        }
    }
}

void reveler_plateau(Jeux *jeu) {
    int r;
    int c;

    for (r = 0; r < TAILLE_PLATEAU; r++) {
        for (c = 0; c < TAILLE_PLATEAU; c++) {
            jeu->plateau[r][c].revele = 1;
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

void initier_plateau(Jeux *jeux) {
    TypeCase cases[TAILLE_PLATEAU * TAILLE_PLATEAU];
    int i = 0;
    int r;
    int c;
    int type;
    int k;

    verifier_ou_corriger_value_base_cartes(jeux);

    for (type = BASILIC; type <= ANTIQUE_VOLEUR; type++) {
        for (k = 0; k < jeux->value_base_cartes[type]; k++) {
            cases[i++] = (TypeCase)type;
        }
    }

    melanger(cases, TAILLE_PLATEAU * TAILLE_PLATEAU);

    i = 0;

    for (r = 0; r < TAILLE_PLATEAU; r++) {
        for (c = 0; c < TAILLE_PLATEAU; c++) {
            jeux->plateau[r][c].type = cases[i++];
            jeux->plateau[r][c].revele = 0;
        }
    }
}

int a_cache_case_adjacente(const Jeux *jeux, Position pos){
    Position dirs[4]= {{-1,0}, {1,0}, {0,-1}, {0,1}};
    int i;
    for (i=0; i<4; i++){
        Position suivant={pos.ligne +dirs[i].ligne, pos.col+dirs[i].col};
        if (dedans(suivant) && jeux->plateau[suivant.ligne][suivant.col].revele == 0) {
            return 1;
        }
    }
    return 0;
}

int a_cache_quelquechose(const Jeux *jeux) {
    int r;
    int c;

    for (r = 0; r < TAILLE_PLATEAU; r++) {
        for (c = 0; c < TAILLE_PLATEAU; c++) {
            if (jeux->plateau[r][c].revele == 0) {
                return 1;
            }
        }
    }

    return 0;
}


Position choix_case_adjacente(const Jeux *jeux, Position pos){
Position dirs[4]={{-1,0},{1,0},{0,-1},{0,1}};
Position options[4];
int compte = 0;
int i;

for (i =0; i < 4; i++){
    Position suivant ={pos.ligne+ dirs[i].ligne, pos.col + dirs[i].col};
        if (dedans(suivant)&&jeux->plateau[suivant.ligne][suivant.col].revele == 0){
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


Position choisir_une_case_cachee(const Jeux *jeux) {
    Position options[TAILLE_PLATEAU * TAILLE_PLATEAU];
    int compte = 0;
    int r;
    int c;
    int choix;

    for (r = 0; r < TAILLE_PLATEAU; r++) {
        for (c = 0; c < TAILLE_PLATEAU; c++) {
            if (jeux->plateau[r][c].revele == 0) {
                options[compte++] = (Position){r, c};
            }
        }
    }

    printf("\nCases cachees possibles :\n");

    for (r = 0; r < compte; r++) {
        printf("%d. (%d, %d)\n",
               r + 1,
               options[r].ligne,
               options[r].col);
    }

    choix = read_int("Choix de la case : ", 1, compte);

    return options[choix - 1];
}

void echanger_totem(Jeux *jeux, Position totem_pos) {
    Position vise;
    TypeCase temp;

    if (!a_cache_quelquechose(jeux)) {
        printf(JAUNE "Aucune case cachee disponible pour echanger le totem.\n" RESET);
        return;
    }

    printf(JAUNE "Choisis une case cachee pour echanger avec le totem.\n" RESET);

    vise = choisir_une_case_cachee(jeux);

    temp = jeux->plateau[totem_pos.ligne][totem_pos.col].type;
    jeux->plateau[totem_pos.ligne][totem_pos.col].type =
        jeux->plateau[vise.ligne][vise.col].type;
    jeux->plateau[vise.ligne][vise.col].type = temp;

    jeux->plateau[totem_pos.ligne][totem_pos.col].revele = 0;
    jeux->plateau[vise.ligne][vise.col].revele = 0;

    printf(JAUNE "Le totem a ete deplace.\n" RESET);
}

void charger_stats(Stats stats[], int *compte) {
    FILE *file;
    char ligne[128];

    *compte = 0;

    file = fopen(STATS_FILE, "r");

    if (file == NULL) {
        return;
    }

    while (fgets(ligne, sizeof(ligne), file) != NULL && *compte < MAX_STATS) {
        Stats s;

        if (sscanf(ligne, "%31[^;];%d;%d",
                   s.nom,
                   &s.parties,
                   &s.victoires) == 3) {
            stats[*compte] = s;
            (*compte)++;
        }
    }

    fclose(file);
}

void sauvegarder_stats(const Stats stats[], int compte) {
    FILE *file;
    int i;

    file = fopen(STATS_FILE, "w");

    if (file == NULL) {
        printf(ROUGE "Impossible d'ecrire le fichier de statistiques.\n" RESET);
        return;
    }

    for (i = 0; i < compte; i++) {
        fprintf(file, "%s;%d;%d\n",
                stats[i].nom,
                stats[i].parties,
                stats[i].victoires);
    }

    fclose(file);
}

void ajouter_mettre_a_jour_stats(Stats stats[],
                                 int *compte,
                                 const char *nom,
                                 int ajouter_partie,
                                 int ajouter_victoire) {
    int i;

    for (i = 0; i < *compte; i++) {
        if (strcmp(stats[i].nom, nom) == 0) {
            stats[i].parties += ajouter_partie;
            stats[i].victoires += ajouter_victoire;
            return;
        }
    }

    if (*compte < MAX_STATS) {
        strncpy(stats[*compte].nom, nom, MAX_NOM - 1);
        stats[*compte].nom[MAX_NOM - 1] = '\0';
        stats[*compte].parties = ajouter_partie;
        stats[*compte].victoires = ajouter_victoire;
        (*compte)++;
    }
}

void maj_date_post_jeux(const Jeux *jeux) {
    Stats stats[MAX_STATS];
    int compte = 0;
    int i;

    charger_stats(stats, &compte);

    for (i = 0; i < jeux->nombre_joueur; i++) {
        ajouter_mettre_a_jour_stats(stats,
                                    &compte,
                                    jeux->joueurs[i].nom,
                                    1,
                                    0);
    }

    if (jeux->gagnant >= 0) {
        ajouter_mettre_a_jour_stats(stats,
                                    &compte,
                                    jeux->joueurs[jeux->gagnant].nom,
                                    0,
                                    1);
    }

    sauvegarder_stats(stats, compte);
}

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

void init_joueurs(Jeux *jeux) {
    int i;

    jeux->nombre_joueur = read_int("Nombre de joueurs (2 a 4) : ", 2, 4);

    for (i = 0; i < jeux->nombre_joueur; i++) {
        char prompt[100];

        snprintf(prompt, sizeof(prompt), "Nom du joueur %d : ", i + 1);
        read_text(prompt, jeux->joueurs[i].nom, TAILLE_NOM_MAX);

        jeux->joueurs[i].id_class = (ClasseJoueur)i;
        jeux->joueurs[i].depart = position_depart(i);

        respawn_joueur(&jeux->joueurs[i]);
    }

    configurer_value_base_cartes(jeux);
}

void montrer_stats(void) {
    Stats stats[MAX_STATS];
    int compte = 0;
    int i;

    charger_stats(stats, &compte);

    printf(GRAS "\n=== STATISTIQUES ===\n" RESET);

    if (compte == 0) {
        printf("Aucune statistique enregistree.\n");
        return;
    }

    for (i = 0; i < compte; i++) {
        printf("- %s : %d partie(s), %d victoire(s)\n",
               stats[i].nom,
               stats[i].parties,
               stats[i].victoires);
    }
}


int demande_replay(void) {
    return read_int(
        "\n1. Rejouer avec les memes joueurs\n"
        "2. Retour au menu principal\n"
        "Choix : ",
        1,
        2
    );
}


void jouer_jeux(Jeux *jeux) {
    int i;

    initier_plateau(jeux);

    jeux->gagnant = -1;
    jeux->heure_depart = time(NULL);

    while (jeux->gagnant == -1) {
        for (i = 0; i < jeux->nombre_joueur; i++) {
            Joueur *joueur = &jeux->joueurs[i];
            int tour_termine = 0;

            respawn_joueur(joueur);
            ecrire_tour(joueur);

            while (!tour_termine && jeux->gagnant == -1) {
                Position choisi;
                TypeCase type_case;

                ecrire_joueurs(jeux, i);
                ecrire_plateau(jeux, joueur, 0);
                ecrire_legende();

                printf("\nEtat du joueur : coffre=%s, arme antique=%s\n",
                       joueur->a_un_tresor ? "oui" : "non",
                       joueur->a_un_antique ? "oui" : "non");

                if (joueur->peut_tp) {
                    if (!a_cache_quelquechose(jeux)) {
                        printf(JAUNE "Plus aucune case cachee. Fin du tour.\n" RESET);
                        tour_termine = 1;
                        break;
                    }

                    printf(JAUNE "Portail actif : tu peux choisir n'importe quelle case cachee.\n" RESET);
                } else {
                    if (!a_cache_case_adjacente(jeux, joueur->pos)) {
                        printf(JAUNE "Aucune case cachee autour de toi. Tu es bloque.\n" RESET);
                        tour_termine = 1;
                        break;
                    }
                }

                joueur->arme = (Arme)(read_int(
                    "\nChoisis ton arme :\n"
                    "1. 🛡️ Bouclier\n"
                    "2. 🔦 Torche\n"
                    "3. 🪓 Hache\n"
                    "4. 🏹 Arc\n"
                    "Choix : ",
                    1,
                    4
                ) - 1);

                printf(CYAN "Arme choisie : %s %s\n" RESET,
                       emoji_arme(joueur->arme),
                       nom_arme(joueur->arme));

                if (joueur->peut_tp) {
                    choisi = choisir_une_case_cachee(jeux);
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

                if (est_monstre(type_case)) {
                    if (bonne_arme(joueur->arme, type_case)) {
                        printf(VERT "Bonne arme : monstre vaincu.\n" RESET);
                    } else {
                        printf(ROUGE "Mauvaise arme : %s est vaincu.\n" RESET,
                               joueur->nom);
                        joueur->en_vie = 0;
                        tour_termine = 1;
                    }

                } else if (type_case == TRESOR) {
                    printf(VERT "Bravo : tu as trouve un coffre 💰.\n" RESET);
                    joueur->a_un_tresor = 1;

                } else if (type_case == PORTAIL) {
                    printf(JAUNE "Portail trouve 🌀 : le prochain deplacement sera libre.\n" RESET);
                    joueur->peut_tp = 1;

                } else if (type_case == TOTEM) {
                    printf(JAUNE "Totem de transmutation 🗿 : fin du tour.\n" RESET);
                    echanger_totem(jeux, choisi);
                    tour_termine = 1;

                } else {
                    if (est_ce_antique(joueur->id_class, type_case)) {
                        printf(VERT "Bravo : tu as trouve ton arme antique %s.\n" RESET,
                               code_case(type_case));
                        joueur->a_un_antique = 1;
                    } else {
                        printf(JAUNE "Tu as trouve une arme antique, mais pas la tienne.\n" RESET);
                    }
                }

                if (joueur->a_un_tresor && joueur->a_un_antique) {
                    jeux->gagnant = i;
                    break;
                }
            }

            if (jeux->gagnant != -1) {
                break;
            }

            cache_plateau(jeux);
            printf("\nFin du tour de %s.\n", joueur->nom);
        }
    }

    jeux->duree = difftime(time(NULL), jeux->heure_depart);

    reveler_plateau(jeux);

    printf(GRAS VERT "\n=========== FIN DE PARTIE ===========\n" RESET);
    printf("Vainqueur : %s %s\n",
           emoji_classe(jeux->joueurs[jeux->gagnant].id_class),
           jeux->joueurs[jeux->gagnant].nom);
    printf("Duree : %.0f seconde(s)\n", jeux->duree);

    ecrire_plateau(jeux, &jeux->joueurs[jeux->gagnant], 1);
    ecrire_legende();

    maj_date_post_jeux(jeux);
}