#include "pgp.h"
#include "types.h"

#ifndef __COMMANDES_H
#define __COMMANDES_H

int teste_commande_general(int num,char** chaineCarac);
void quitte_pass_phrase_incoherente();
void quitte_pas_probleme();
void quitte_demande_invalide();
void quitte_suppression_impossible(char* s);

#endif
