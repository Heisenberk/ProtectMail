#include "types.h"

#ifndef __GESTION_CLES_H
#define __GESTION_CLES_H

void genere_cle_privee();
void genere_cle_publique();
void cree_pass_phrase(char* buffer1);
void demande_pass_phrase(unsigned char* hash);

#endif

