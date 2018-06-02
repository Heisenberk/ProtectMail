/**
 * \file gestion_cles.h
 * \author Claire Baskevitch - Clément Caumes
 * \date 2017
 * \brief contient la signature des fonctions concernant le chiffrement/déchiffrement et les cles 
 */

#include "types.h"
#include <stdlib.h>
#include <gmp.h>

#ifndef __GESTION_CLES_H
#define __GESTION_CLES_H

/**
 * \struct CLE
 * \brief structure qui contient la cle de session de taille 16 (+1 pour '\0')
 * aux travaux des threads 
 * */
typedef struct {
	char session[17];	/*!< chaine de caracteres qui represente la cle de session */
} CLE;

//void encrypt_rsa_fic(char* nomFichier,mpz_t n,mpz_t e);
//void decrypt_rsa_fic(char* nomFichier,mpz_t n,mpz_t d);

/**
 * \fn void encrypt_rsa_chaine(char* chaine,int taille,FILE* new,mpz_t n, mpz_t e)
 * \brief chiffrement rsa de la cle de session et ecriture dans le fichier 'new'
 * \param *chaine chaine de caracteres a chiffrer 
 * \param taille taille de la chaine de caracteres a chiffrer
 * \param *new fichier ou ecrire le chiffrement
 * \param n cle publique (n)
 * \param e cle publique (e)
 * */
void encrypt_rsa_chaine(char *chaine, int taille, FILE * new, mpz_t n, mpz_t e);

/**
 * \fn void decrypt_rsa_chaine(char* chaine,int taille,FILE* new,mpz_t n, mpz_t d)
 * \brief dechiffrement rsa de la cle de session et ecriture dans le fichier 'new'
 * \param *chaine chaine de caracteres a dechiffrer 
 * \param taille taille de la chaine de caracteres a dechiffrer
 * \param *new fichier ou ecrire le dechiffrement
 * \param n cle privee (n)
 * \param e cle privee (d)
 * */
void decrypt_rsa_chaine(char *chaine, int taille, FILE * new, mpz_t n, mpz_t d);

/**
 * \fn void genere_cle_privee(mpz_t n,mpz_t d)
 * \brief generation d'une cle privee
 * \param n cle privee (n)
 * \param d cle privee (d)
 * */
void genere_cle_privee(mpz_t n, mpz_t d);

/**
 * \fn void genere_cle_publique(mpz_t n,mpz_t e)
 * \brief generation d'une cle publique
 * \param n cle privee (n)
 * \param d cle privee (e)
 * */
void genere_cle_publique(mpz_t n, mpz_t e);

/**
 * \fn void genere_cles()
 * \brief generation des cles publiques et privees
 * */
void genere_cles();

/**
 * \fn void cree_pass_phrase(char* buffer1)
 * \brief stocke dans buffer1 la pass phrase choisie
 * \param *buffer1 chaine de caracteres ou sera stocke la pass phrase
 * */
void cree_pass_phrase(char *buffer1);

/**
 * \fn CLE genere_cle_session()
 * \brief genere aleatoirement une cle de session
 * \return retourne la cle generee
 * */
CLE genere_cle_session();

/**
 * \fn void encrypt_session(char* nomFichier1,FILE* new,CLE clef)
 * \brief chiffrement XOR de la cle de session avec les caracteres du fichier
 * \param *nomFichier1 nom du fichier a chiffrer
 * \param *new fichier ou ecrire le resultat du chiffrement
 * \param clef cle de session pour chiffrer le fichier
 * */
void encrypt_session(char *nomFichier1, FILE * new, CLE clef);

/**
 * \fn void decrypt_session(CLE c,FILE* f_toDecrypt,FILE* f_vierge)
 * \brief dechiffrement XOR de la cle de session avec les caracteres du fichier
 * \param c cle de session pr le dechiffrement
 * \param *f_toDecrypt fichier a dechiffrer
 * \param *f_vierge fichier ou le resultat du dechiffrement
 * */
void decrypt_session(CLE c, FILE * f_toDecrypt, FILE * f_vierge);

/**
 * \fn void decrypt_session_affichage(CLE c,FILE* f_toDecrypt)
 * \brief dechiffrement XOR de la cle de session avec les caracteres du fichier et ecriture sur le terminal
 * \param c cle de session pr le dechiffrement
 * \param *f_toDecrypt fichier a dechiffrer
 * */
void decrypt_session_affichage(CLE c, FILE * f_toDecrypt);

#endif
