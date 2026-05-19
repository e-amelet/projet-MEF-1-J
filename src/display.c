#include <stdio.h>
#include "display.h"

typedef struct {
    const char *nom;
    const char *emoji;
} Info;

typedef struct {
    const char *nom;
    const char *emoji;
    const char *couleur;
} InfoCase;

static const Info CLASSES[] = {
    {"Guerrier", "🛡️"},
    {"Ranger", "🌿"},
    {"Mage", "🧙"},
    {"Voleur", "🥷"}
};

static const Info ARMES[] = {
    {"Bouclier", "🛡️"},
    {"Torche", "🔦"},
    {"Hache", "🪓"},
    {"Arc", "🏹"}
};

static const InfoCase CASES[] = {
    {"Basilic", "🐍", ROUGE},
    {"Zombie", "🧟", ROUGE},
    {"Troll", "👹", ROUGE},
    {"Harpie", "🦅", ROUGE},
    {"Tresor", "💰", VERT},
    {"Portail", "🌀", JAUNE},
    {"Totem", "🗿", JAUNE},
    {"Antique guerrier", "🔥", VIOLET},
    {"Antique ranger", "🪄", VIOLET},
    {"Antique mage", "📖", VIOLET},
    {"Antique voleur", "🗡️", VIOLET}
};

const char *nom_classe(ClasseJoueur id_class) {
    if (id_class < GUERRIER || id_class > VOLEUR) {
        return "Inconnue";
    }

    return CLASSES[id_class].nom;
}

const char *emoji_classe(ClasseJoueur id_class) {
    if (id_class < GUERRIER || id_class > VOLEUR) {
        return "❔";
    }

    return CLASSES[id_class].emoji;
}

const char *nom_arme(Arme arme) {
    if (arme < BOUCLIER || arme > ARC) {
        return "Inconnue";
    }

    return ARMES[arme].nom;
}

const char *emoji_arme(Arme arme) {
    if (arme < BOUCLIER || arme > ARC) {
        return "❔";
    }

    return ARMES[arme].emoji;
}

const char *nom_type_case(TypeCase type) {
    if (type < BASILIC || type > ANTIQUE_VOLEUR) {
        return "Inconnu";
    }

    return CASES[type].nom;
}

const char *code_case(TypeCase type) {
    if (type < BASILIC || type > ANTIQUE_VOLEUR) {
        return "❔";
    }

    return CASES[type].emoji;
}

const char *couleur_case(TypeCase type) {
    if (type < BASILIC || type > ANTIQUE_VOLEUR) {
        return BLANC;
    }

    return CASES[type].couleur;
}

void ecrire_tour(const Joueur *joueur) {
    printf("\n");
    printf(BLEU "===============================================\n" RESET);
    printf(GRAS CYAN "Tour de %s %s" RESET " (%s)\n",
           emoji_classe(joueur->id_class),
           joueur->nom,
           nom_classe(joueur->id_class));
    printf(BLEU "===============================================\n" RESET);
}

void ecrire_joueurs(const Jeux *jeux, int joueur_actuel) {
    int i;

    printf(GRAS "\nJoueurs :\n" RESET);

    for (i = 0; i < jeux->nombre_joueur; i++) {
        if (i == joueur_actuel) {
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