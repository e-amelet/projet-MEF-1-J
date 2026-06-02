#include <stdio.h>
#include "configue_carte.h"
#include "display.h"
#include "utils.h"

static const int VALEURS_OFFICIELLES[NB_TYPE_CASES] = {
    3,  /* BASILIC */
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

/* Initialise le tableau valeur_base_cartes avec les valeurs officielles.
Chaque type de case reçoit sa valeur par défaut définie dans VALEURS_OFFICIELLES.*/
void initier_valeur_base_carte(Jeux *jeux) {
    int i;

    for (i = 0; i < NB_TYPE_CASES; i++) { 
        jeux->value_base_cartes[i] = VALEURS_OFFICIELLES[i];
    }
}

/* Calcule la somme totale des cartes présentes sur le plateau en additionnant
 les valeurs de tous les types de cases. */
int total_valeur_base_cartes(const Jeux *jeux) {
    int total = 0;
    int i;

    for (i = 0; i < NB_TYPE_CASES; i++) {
        total += jeux->value_base_cartes[i];
    }

    return total;
}

/*Affiche la configuration actuelle des cartes (code,type,nb de cartes)
 et affiche le total par rapport à la taille du plateau.*/
void afficher_valeur_base_cartes(const Jeux *jeux) {
    int i;

    printf(GRAS "\nConfiguration actuelle des cartes :\n" RESET);

    for (i = BASILIC; i <= ANTIQUE_VOLEUR; i++) {
        printf("- %s %s : %d\n",
               code_case((TypeCase)i),
               nom_type_case((TypeCase)i),
               jeux->value_base_cartes[i]);
    }

    printf("Total : %d / %d\n",
           total_valeur_base_cartes(jeux),
           TAILLE_PLATEAU * TAILLE_PLATEAU);
}

/* 
 * Vérifie si la configuration des cartes est valide 
 (si total=taille plateau et chaque type respecte sa valeur). retourne 1 si valide, 0 sinon.*/    
static int value_base_cartes_est_valide(const Jeux *jeux) {
    int i;
    int total = total_valeur_base_cartes(jeux);
    int valide = 1;
    int total_attendu = TAILLE_PLATEAU * TAILLE_PLATEAU;

    /* Vérification du total de cartes */
    if (total < total_attendu) {
        printf(ROUGE "\nErreur : il n'y a pas assez de cartes sur le terrain.\n" RESET);
        printf("Total actuel : %d / %d\n", total, total_attendu);
        valide = 0;
    } else if (total > total_attendu) {
        printf(ROUGE "\nErreur : il y a trop de cartes sur le terrain.\n" RESET);
        printf("Total actuel : %d / %d\n", total, total_attendu);
        valide = 0;
    }
/* Vérification de chaque type de case */
    for (i = BASILIC; i <= ANTIQUE_VOLEUR; i++) {
        int attendu = VALEURS_OFFICIELLES[i];
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

/* L'utilisateur peut saisir manuellement le nombre de cartes pour chaque type
 et la taille entre 0 et la taille du plateau. Affiche un message d'erreur si
  les valeurs ne sont pas valides et invite à ressaisir les valeurs.*/
static void saisir_value_base_cartes(Jeux *jeux) {
    int i;
    char prompt[120];

    printf(JAUNE "\nModification du nombre de cartes.\n" RESET);

    for (i = BASILIC; i <= ANTIQUE_VOLEUR; i++) {
        snprintf(prompt,
                 sizeof(prompt),
                 "Nombre de cartes pour %s %s : ",
                 code_case((TypeCase)i),
                 nom_type_case((TypeCase)i));

        jeux->value_base_cartes[i] = lire_entier(prompt, 0, TAILLE_PLATEAU * TAILLE_PLATEAU);
    }
}

/* Vérifie la configuration des cartes et force l'utilisateur à corriger
  tant qu'elle est invalide (soit automatiquement, soit manuellement). */
 
void verifier_ou_corriger_valeur_base_cartes(Jeux *jeux) {
    int choix;

    while (!value_base_cartes_est_valide(jeux)) {
        afficher_valeur_base_cartes(jeux);

        choix = lire_entier(
            "\nLa configuration des cartes n'est pas valide.\n"
            "1. Corriger automatiquement avec les valeurs officielles\n"
            "2. Ressaisir les valeurs manuellement\n"
            "Choix : ",
            1,
            2
        );

        if (choix == 1) {
            initier_valeur_base_carte(jeux);
            printf(VERT "Les valeurs officielles ont ete restaurees.\n" RESET);
        } else {
            saisir_value_base_cartes(jeux);
        }
    }
}

/* Fonction principale de configuration des cartes (initialise les valeurs officielles,
  propose modification, vérifie et corrige, affiche la configuration finale) */
void configurer_valeur_base_cartes(Jeux *jeux) {
    int choix;

    initier_valeur_base_carte(jeux);

    choix = lire_entier(
        "\nConfiguration des cartes :\n"
        "1. Garder les valeurs officielles\n"
        "2. Modifier les valeurs manuellement\n"
        "Choix : ",
        1,
        2
    );

    if (choix == 2) {
        saisir_value_base_cartes(jeux);
    }

    verifier_ou_corriger_valeur_base_cartes(jeux);
    afficher_valeur_base_cartes(jeux);
}