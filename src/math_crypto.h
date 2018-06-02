/**
 * \file math_crypto.h
 * \author Claire Baskevitch - Clément Caumes
 * \date 2017
 * \brief contient les signatures des fonctions concernant les mathematiques en cryptographie
 */

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

/**
 * \struct SHA1
 * \brief structure qui contient les informations pour le hash SHA-1
 * */
typedef struct {
	int nbBlocs;		/*!< nombres de blocs du hash */
	uint32_t *mot[16];	/*!< pointeur sur des tableaux de 16 qui contient le hash final */
	uint32_t registre1[5];	/*!< A,B,C,D,E */
	uint32_t registre2[5];	/*!< H0,H1,H2,H3,H4,H5 */
	uint32_t W[80];		/*!< W[80] */
} SHA1;

/**
 * \fn void initialise_memoire(mpz_t p,mpz_t q,mpz_t n,mpz_t z,mpz_t e,mpz_t d)
 * \brief initialise p,q,n,z,e,d
 * \param p,q,n,z,e,d sont les valeurs utiles pour determiner les cles publiques/privees RSA
 * */
void initialise_memoire(mpz_t p, mpz_t q, mpz_t n, mpz_t z, mpz_t e, mpz_t d);

/**
 * \fn void libere_memoire(mpz_t p,mpz_t q,mpz_t n,mpz_t z,mpz_t e,mpz_t d)
 * \brief libere la memoire de p,q,n,z,e,d
 * \param p,q,n,z,e,d sont les valeurs utiles pour determiner les cles publiques/privees RSA
 * */
void libere_memoire(mpz_t p, mpz_t q, mpz_t n, mpz_t z, mpz_t e, mpz_t d,
		    gmp_randstate_t state);

/**
 * \fn void determine_premier(mpz_t p,gmp_randstate_t state,int choix)
 * \brief fonction qui met dans p un nombre aleatoire
 * \param p nombre qui sera le nombre aleatoire
 * \param state seed pour le pseudo aleatoire
 * \param choix parametre utile pour le pseudo aleatoire
 * */
void determine_premier(mpz_t p, gmp_randstate_t state, int choix);

/**
 * \fn void determine_n(mpz_t p,mpz_t q,mpz_t n)
 * \brief fonction qui calcule le produit p et q
 * \param p nombre 
 * \param q nombre
 * \param n resultat du produit p et q
 * */
void determine_n(mpz_t p, mpz_t q, mpz_t n);

/**
 * \fn void determine_z(mpz_t p,mpz_t q,mpz_t z)
 * \brief fonction qui calcule le produit p-1 et q-1
 * \param p nombre 
 * \param q nombre
 * \param z resultat du produit p-1 et q-1
 * */
void determine_z(mpz_t p, mpz_t q, mpz_t z);

/**
 * \fn void determine_e(mpz_t z,gmp_randstate_t state,mpz_t e)
 * \brief fonction qui calcule e tq pgcd(e,z)=1
 * \param z nombre 
 * \param state graine pour le pseudo aleatoire
 * \param e nombre
 * */
void determine_e(mpz_t z, gmp_randstate_t state, mpz_t e);

/**
 * \fn void determine_d(mpz_t p,mpz_t q,mpz_t n,mpz_t z,mpz_t e,mpz_t d,gmp_randstate_t state)
 * \brief fonction qui calcule d=e^-1 mod n
 * \param p nombre
 * \param q nombre
 * \param e nombre
 * \param state graine pour le pseudo aleatoire
 * */
void determine_d(mpz_t p, mpz_t q, mpz_t n, mpz_t z, mpz_t e, mpz_t d,
		 gmp_randstate_t state);

/**
 * \fn int random_int(gmp_randstate_t state)
 * \brief fonction qui cree un entier aleatoirement
 * \param state graine pour le pseudo aleatoire
 * \return retourne l'entier cree
 * */
int random_int(gmp_randstate_t state);

/**
 * \fn int xor(int i,int j)
 * \brief fonction qui fait i XOR j
 * \param i entier
 * \param j entier
 * \return retourne i XOR j
 * */
int xor(int i, int j);

/**
 * \fn void myitoa(int in,char* out,int longueur)
 * \brief fonction qui convertit un entier en chaine de caracteres
 * \param in entier a convertir
 * \param out chaine de caracteres de l'entier converti 
 * \param longueur taille de la chaine de caracteres
 * */
void myitoa(int in, char *out, int longueur);

/**
 * \fn uint32_t f(uint32_t B,uint32_t C,uint32_t D,int t)
 * \brief fonction logique f du hash SHA-1
 * \param B parametre 1
 * \param C parametre 2
 * \param D parametre 3
 * \param t parametre 4
 * \return resultat de la fonction logique f
 * */
uint32_t f(uint32_t B, uint32_t C, uint32_t D, int t);

/**
 * \fn uint32_t k(int t)
 * \brief fonction logique k du hash SHA-1
 * \param t parametre 
 * \return resultat de la fonction logique k
 * */
uint32_t k(int t);

/**
 * \fn uint32_t shift(int n,uint32_t X)
 * \brief fonction de shift qui decale X de n rangs
 * \param n nombre de fois ou l'on va shift
 * \param X nombre a shifter
 * \return resultat de n shift sur X
 * */
uint32_t shift(int n, uint32_t X);

/**
 * \fn int converbi(char* chaine)
 * \brief fonction qui convertit en binaire une chaine de caracteres
 * \param chaine chaine a convertir en binaire
 * \return resultat de la conversion en binaire
 * */
int converbi(char *chaine);

/**
 * \fn uint32_t converhexa(char* chaine)
 * \brief fonction qui convertit en hexadecimal une chaine de caracteres
 * \param chaine chaine de caracteres a convertir en hexadecimal
 * \return resultat de la conversion en hexadecimal
 * */
uint32_t converhexa(char *chaine);

/**
 * \fn void remplit_zeros(char* t,int taille)
 * \brief fonction qui remplit t de 0
 * \param chaine chaine de caracteres a remplir de 0
 * \param taille taille de la chaine de caracteres
 * */
void remplit_zeros(char *t, int taille);

/**
 * \fn void remplit_valeur(char* t,int rang,int val)
 * \brief fonction qui remplit t de val
 * \param chaine chaine de caracteres a remplir de val
 * \param taille taille de la chaine de caracteres
 * */
void remplit_valeur(char *t, int rang, int val);

/**
 * \fn void affiche(char* message,int longueur)
 * \brief fonction qui affiche la chaine de caracteres message
 * \param message chaine de caracteres a afficher
 * \param longueur taille de la chaine de caracteres
 * */
void affiche(char *message, int longueur);

/**
 * \fn SHA1 init_registres(SHA1 hash)
 * \brief fonction qui initialise hash avec les registres
 * \param hash hash SHA-1 dont les registres doivent etre initialises
 * \return futur hash ou les registres seront initialises
 * */
SHA1 init_registres(SHA1 hash);

/**
 * \fn SHA1 init_16W(SHA1 hash,int rang)
 * \brief fonction qui initialise hash avec les mots W
 * \param hash hash SHA-1 
 * \param rang rang ou il faut initialises les mots W
 * \return futur hash correctement initialise
 * */
SHA1 init_16W(SHA1 hash, int rang);

/**
 * \fn SHA1 init_sha1(char* message)
 * \brief fonction qui initialise hash a partir de message
 * \param message chaine de caracteres que l'on veut hash
 * \return futur hash initialise
 * */
SHA1 init_sha1(char *message);

/**
 * \fn void process_sha1(SHA1 hash,char* out)
 * \brief fonction qui fait les differentes manipulations binaires pour obtenir le hash final
 * \param hash hash qui vient d'etre initialise
 * \param out chaine de caracteres
 * */
void process_sha1(SHA1 hash, char *out);

/**
 * \fn void done_sha1(SHA1 hash)
 * \brief fonction qui libere la memoire reservee
 * \param hash hash SHA-1
 * */
void done_sha1(SHA1 hash);

#endif
