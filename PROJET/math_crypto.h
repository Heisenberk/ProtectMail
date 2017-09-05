//VERIFIE

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

#endif
