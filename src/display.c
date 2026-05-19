#include <stdio.h>
#include "display.h"

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