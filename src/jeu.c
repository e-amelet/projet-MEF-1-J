#include <stdio.h>
#include <time.h>
#include "configue_carte.h"
#include "jeu.h"
#include "utils.h"
#include "display.h"
#include "regles.h"
#include "plateau.h"
#include "stats.h"


/* Permet au joueur de choisir une arme parmi 4 options et retourne
  l'arme choisie.*/
static Arme choisir_arme(void) {
    return (Arme)(lire_entier(
        "\nChoisis ton arme :\n"
        "1. 🛡️ Bouclier\n"
        "2. 🔦 Torche\n"
        "3. 🪓 Hache\n"
        "4. 🏹 Arc\n"
        "Choix : ",
        1,
        4
    ) - 1);
}

/*Affiche l'état actuel du joueur (presence du trésor et possession
  de l'arme antique) avant chaque tour.*/
static void afficher_etat_joueur(const Joueur *joueur) {
    printf("\nEtat du joueur : coffre=%s, arme antique=%s\n",
           joueur->a_un_tresor ? "oui" : "non",
           joueur->a_un_antique ? "oui" : "non");
}

/* Révèle une case du plateau (met la case comme révélée,
  maj position du joueur et affiche le type de case decouverte). */
static void reveler_case(Jeux *jeux, Joueur *joueur, Position choisi) {
    TypeCase type_case;

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
}

/*Applique l'effet de la case sur le joueur (combat contre les monstres, trésor,
 portail, totem, arme antique)*/
static void appliquer_effet_case(Jeux *jeux, Joueur *joueur, TypeCase type_case,
         Position choisi, int *tour_termine) {
        /* Cas : monstre */
    if (est_monstre(type_case)) {
        if (bonne_arme(joueur->arme, type_case)) {
            printf(VERT "Bonne arme : monstre vaincu.\n" RESET);
        } else {
            printf(ROUGE "Mauvaise arme : %s est vaincu.\n" RESET,
                   joueur->nom);
            joueur->en_vie = 0;
            *tour_termine = 1;
        }

         /* Cas : Tresor */
    } else if (type_case == TRESOR) {
        printf(VERT "Bravo : tu as trouve un coffre 💰.\n" RESET);
        joueur->a_un_tresor = 1;

        /* Cas : portail */
    } else if (type_case == PORTAIL) {
        printf(JAUNE "Portail trouve 🌀 : le prochain deplacement sera libre.\n" RESET);
        joueur->peut_tp = 1;

        /* Cas : totem */
    } else if (type_case == TOTEM) {
        printf(JAUNE "Totem de transmutation 🗿 : fin du tour.\n" RESET);
        echanger_totem(jeux, choisi);
        *tour_termine = 1;

        /* Cas : arme antique correspondant au joueur */
    } else if (est_ce_antique(joueur->id_class, type_case)) {
        printf(VERT "Bravo : tu as trouve ton arme antique %s.\n" RESET,
               code_case(type_case));
        joueur->a_un_antique = 1;

            /* Cas : arme antique d’un autre joueur */
    } else {
        printf(JAUNE "Tu as trouve une arme antique, mais pas la tienne.\n" RESET);
    }
}

/*Détermine la prochaine case à jouer (gestion du portail, cases adjacentes,
  blocage si aucune disponible) */
static Position choisir_prochaine_case(Jeux *jeux, Joueur *joueur, int *tour_termine) {
    Position choisi;

    /* Cas portail actif */
    if (joueur->peut_tp) {
        if (!a_cache_quelquechose(jeux)) {
            printf(JAUNE "Plus aucune case cachee. Fin du tour.\n" RESET);
            *tour_termine = 1;
            return joueur->pos;
        }

        printf(JAUNE "Portail actif : tu peux choisir n'importe quelle case cachee.\n" RESET);
        choisi = choisir_une_case_cachee(jeux);
        joueur->peut_tp = 0;
        return choisi;
    }

    /* Cas normal : déplacement adjacent */
    if (!a_cache_case_adjacente(jeux, joueur->pos)) {
        printf(JAUNE "Aucune case cachee autour de toi. Tu es bloque.\n" RESET);
        *tour_termine = 1;
        return joueur->pos;
    }

    return choix_case_adjacente(jeux, joueur->pos);
}

/*Gère un tour complet d’un joueur(affichage etat, choix arme et deplacement,
  revelation case, application effet et condition de victoires)*/
static void jouer_tour_joueur(Jeux *jeux, int index_joueur) {
    Joueur *joueur = &jeux->joueurs[index_joueur];
    int tour_termine = 0;

    respawn_joueur(joueur);
    ecrire_tour(joueur);

    while (!tour_termine && jeux->gagnant == -1) {
        Position choisi;
        TypeCase type_case;

        ecrire_joueurs(jeux, index_joueur);
        ecrire_plateau(jeux, joueur, 0);
        ecrire_legende();
        afficher_etat_joueur(joueur);

        joueur->arme = choisir_arme();

        printf(CYAN "Arme choisie : %s %s\n" RESET,
               emoji_arme(joueur->arme),
               nom_arme(joueur->arme));

        choisi = choisir_prochaine_case(jeux, joueur, &tour_termine);

        if (tour_termine) {
            break;
        }

        reveler_case(jeux, joueur, choisi);

        type_case = jeux->plateau[choisi.ligne][choisi.col].type;

        appliquer_effet_case(jeux, joueur, type_case, choisi, &tour_termine);

        /* Condition de victoire */
        if (joueur->a_un_tresor && joueur->a_un_antique) {
            jeux->gagnant = index_joueur;
            break;
        }
    }

    /* Fin du tour */
    if (jeux->gagnant == -1) {
        cache_plateau(jeux);
        printf("\nFin du tour de %s.\n", joueur->nom);
    }
}
/* Termine la partie (calcul la durée, affiche le plateau final,
  affiche le gagnant, met à jour les stats)*/
static void terminer_partie(Jeux *jeux) {
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

/*Initialise tous les joueurs (nb de joueurs, nom, classe, position de départ,
  état initial) et configure les cartes. */
void init_joueurs(Jeux *jeux) {
    int i;

    jeux->nombre_joueur = lire_entier("Nombre de joueurs (2 a 4) : ", 2, 4);

    for (i = 0; i < jeux->nombre_joueur; i++) {
        char prompt[100];

        snprintf(prompt, sizeof(prompt), "Nom du joueur %d : ", i + 1);
        read_text(prompt, jeux->joueurs[i].nom, TAILLE_NOM_MAX);

        jeux->joueurs[i].id_class = (ClasseJoueur)i;
        jeux->joueurs[i].depart = position_depart(i);

        respawn_joueur(&jeux->joueurs[i]);
    }

    configurer_valeur_base_cartes(jeux);
}

/* Propose une option après une partie (rejouer ou retourner au menu)*/
int demande_replay(void) {
    return lire_entier(
        "\n1. Rejouer avec les memes joueurs\n"
        "2. Retour au menu principal\n"
        "Choix : ",
        1,
        2
    );
}

/*Boucle principale de jeu (initialise les joueurs et les cartes, puis boucle 
sur les tours des joueurs) detecte le gagnant et termine la partie*/
void jouer_jeux(Jeux *jeux) {
    int i;

    initier_plateau(jeux);

    jeux->gagnant = -1;
    jeux->heure_depart = time(NULL);

    while (jeux->gagnant == -1) {
        for (i = 0; i < jeux->nombre_joueur; i++) {
            jouer_tour_joueur(jeux, i);

            if (jeux->gagnant != -1) {
                break;
            }
        }
    }

    terminer_partie(jeux);
}