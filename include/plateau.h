#ifndef PLATEAU_H
#define PLATEAU_H

#include "jeu.h"

void initier_plateau(Jeux *jeux); 
void cache_plateau(Jeux *jeux);
void reveler_plateau(Jeux *jeux);

int a_cache_case_adjacente(const Jeux *jeux, Position pos); /*sert pour savoir si des cases a côté son pas encore decouvert quand il doit choisir son déplacement*/
int a_cache_quelquechose(const Jeux *jeux);   
                                                   
Position choix_case_adjacente(const Jeux *jeux, Position pos);  /*1,2,3 ou 4 choix*/
Position choisir_une_case_cachee(const Jeux *jeux);  /*quelle choix on doit proposer au joueur*/

void echanger_totem(Jeux *jeux, Position totem_pos);  

#endif