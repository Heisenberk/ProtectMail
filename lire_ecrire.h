#include "pgp.h"
#include "types.h"

#ifndef __LIRE_ECRIRE_H
#define __LIRE_ECRIRE_H

void affiche_general();
void affiche_commandes();
void affiche_action_pgp(char* nom);
void ecrit_bordure_sup_m_sig(FILE* f);
void ecrit_bordure_inf_m_sig(FILE* f);
void recopie_message(FILE* origin,FILE* new);

void affiche_contenu_fic(char* nomFichier);
void demande_visualisation_message(char* nomFichier);

#endif
