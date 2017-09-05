//VERIFIE

#include "types.h"
#include <gmp.h>

#ifndef __GESTION_CLES_H
#define __GESTION_CLES_H

struct cle{
	char session[17];
};typedef struct cle CLE;

/*void initialise_memoire(mpz_t p,mpz_t q,mpz_t n,mpz_t z,mpz_t e,mpz_t d);
void libere_memoire(mpz_t p,mpz_t q,mpz_t n,mpz_t z,mpz_t e,mpz_t d,gmp_randstate_t state);
void determine_premier(mpz_t p,gmp_randstate_t state,int choix);
void determine_n(mpz_t p,mpz_t q,mpz_t n);
void determine_z(mpz_t p,mpz_t q,mpz_t z);
void determine_e(mpz_t z,gmp_randstate_t state,mpz_t e);
void determine_d(mpz_t p,mpz_t q,mpz_t n,mpz_t z,mpz_t e,mpz_t d,gmp_randstate_t state);
void affiche_cles(mpz_t e,mpz_t d,mpz_t n);
//void genere_cle_privee();
//void genere_cle_publique();

void encrypt_session(char* nomFichier1,FILE* new,CLE clef);
void decrypt_session(CLE c,FILE* f_toDecrypt,FILE* f_vierge);
void decrypt_session_affichage(CLE c,FILE* f_toDecrypt);

void encrypt_rsa_chaine(char* chaine,FILE* new,mpz_t n, mpz_t e);
//void decrypt_rsa_chaine(char* chaine,FILE* f,mpz_t n, mpz_t d);
void decrypt_rsa_chaine(char* chaine,FILE* new,mpz_t n, mpz_t d);
void genere_cles();
void cree_pass_phrase(char* buffer1);
void demande_pass_phrase(unsigned char* hash);
CLE genere_cle_session();*/


// A SUPPRIMER
/*void affiche_cles(mpz_t e,mpz_t d,mpz_t n){
	gmp_printf("CLE PUBLIQUE (%Zd,%Zd)\n",e,n);
	gmp_printf("CLE PRIVEE   (%Zd,%Zd)\n",d,n);
}*/

void encrypt_rsa_fic(char* nomFichier,mpz_t n,mpz_t e);
void decrypt_rsa_fic(char* nomFichier,mpz_t n,mpz_t d);
void encrypt_rsa_chaine(char* chaine,FILE* new,mpz_t n, mpz_t e);
void decrypt_rsa_chaine(char* chaine,FILE* new,mpz_t n, mpz_t d);

void md5(unsigned char* in,int taille,unsigned char* out);

void genere_cle_privee(mpz_t n,mpz_t d);
void genere_cle_publique(mpz_t n,mpz_t e);
void genere_cles();

void cree_pass_phrase(char* buffer1);
void demande_pass_phrase(unsigned char* hash);

CLE genere_cle_session();
void encrypt_session(char* nomFichier1,FILE* new,CLE clef);
void decrypt_session(CLE c,FILE* f_toDecrypt,FILE* f_vierge);
void decrypt_session_affichage(CLE c,FILE* f_toDecrypt);

#endif

