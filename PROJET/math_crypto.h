//VERIFIE


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include <string.h>
#include "pgp.h"
#include "types.h"
#include "gestion_cles.h"

#ifndef __MATH_CRYPTO_H
#define __MATH_CRYPTO_H

void initialise_memoire(mpz_t p,mpz_t q,mpz_t n,mpz_t z,mpz_t e,mpz_t d);
void libere_memoire(mpz_t p,mpz_t q,mpz_t n,mpz_t z,mpz_t e,mpz_t d,gmp_randstate_t state);
void determine_premier(mpz_t p,gmp_randstate_t state,int choix);
void determine_n(mpz_t p,mpz_t q,mpz_t n);
void determine_z(mpz_t p,mpz_t q,mpz_t z);
void determine_e(mpz_t z,gmp_randstate_t state,mpz_t e);
void determine_d(mpz_t p,mpz_t q,mpz_t n,mpz_t z,mpz_t e,mpz_t d,gmp_randstate_t state);

int random_int(gmp_randstate_t state);
int xor(int i,int j);



// -lm compilation

struct sha1{
	int nbBlocs; 
	uint32_t* mot[16];
	uint32_t registre1[5]; //A,B,C,D,E
	uint32_t registre2[5]; //H0,H1,H2,H3,H4,H5
	uint32_t W[80];
};typedef struct sha1 SHA1;

void myitoa(int in,char* out,int longueur);
uint32_t f(uint32_t B,uint32_t C,uint32_t D,int t);
uint32_t k(int t);
uint32_t shift(int n,uint32_t X);
int converbi(char* chaine);
uint32_t converhexa(char* chaine);
void remplit_zeros(char* t,int taille);
void remplit_valeur(char* t,int rang,int val);
void affiche(char* message,int longueur);
SHA1 init_registres(SHA1 hash);
SHA1 init_16W(SHA1 hash,int rang);
SHA1 init_sha1(char* message);
void process_sha1(SHA1 hash,char* out);
void done_sha1(SHA1 hash);

#endif
