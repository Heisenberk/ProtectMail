//VERIFIE

#include "pgp.h"
#include "types.h"
#include "gestion_cles.h"

#ifndef __ENVOI_H
#define __ENVOI_H

/*//FILE* cree_message_vide(int num,char**chaineCarac);
void genere_cle_privee();
void genere_cle_publique();
void cherche_cle_pub(mpz_t n,mpz_t e);
void encrypt_session(char* nomFichier1,FILE* new,CLE clef);
//void demande_pass_phrase(unsigned char* hash);
void ecrit_message_non_chiffre(int num,char**chaineCarac);
void cree_fichier_chiffre(char* nomFichier);*/

char* concatenation_extension(char* s);
void ecrit_message_non_chiffre(int num,char**chaineCarac);
void cree_fichier_chiffre(char* nomFichier);

#endif
