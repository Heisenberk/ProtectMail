//VERIFIE

#include "pgp.h"
#include "types.h"

#ifndef __COMMANDES_H
#define __COMMANDES_H

/*int teste_commande_general(int num,char** chaineCarac);
void quitte_pass_phrase_incoherente();
void quitte_pas_probleme();
void quitte_demande_invalide();
void quitte_suppression_impossible(char* s);*/

void quitte_commande_introuvable();
void quitte_demande_invalide();
void quitte_fichier_inexistant();
void quitte_fichier_invalide();
void quitte_mauvaise_extension(char* s);
void quitte_pass_phrase_incoherente();
void quitte_suppression_impossible(char* s);
void quitte_pas_probleme();

int teste_mots_identiques(char* s1,char* s2);
int teste_extension_pgp(char* s);
int teste_commande_une_option(int num,char** chaineCarac);
int teste_commande_deux_options(int num,char** chaineCarac);
int teste_commande_trois_options(int num,char** chaineCarac);
int teste_commande_general(int num,char** chaineCarac);

#endif
