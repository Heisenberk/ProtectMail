#include "pgp.h"
#include "types.h"

#ifndef __ENVOI_H
#define __ENVOI_H

//FILE* cree_message_vide(int num,char**chaineCarac);
void genere_cle_privee();
void genere_cle_publique();
void demande_pass_phrase(unsigned char* hash);
void ecrit_message_non_chiffre(int num,char**chaineCarac);

#endif
