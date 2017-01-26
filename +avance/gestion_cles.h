#include "types.h"

#ifndef __GESTION_CLES_H
#define __GESTION_CLES_H

struct cle{
	char session[17];
};typedef struct cle CLE;


//void genere_cle_privee();
//void genere_cle_publique();
void encrypt_rsa_chaine(char* chaine,FILE* new,mpz_t n, mpz_t e);
void genere_cles();
void cree_pass_phrase(char* buffer1);
void demande_pass_phrase(unsigned char* hash);
CLE genere_cle_session();

#endif

