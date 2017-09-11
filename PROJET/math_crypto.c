/**
 * \file math_crypto.c
 * \author Claire Baskevitch - Clément Caumes
 * \date 2017
 * \brief contient les fonctions concernant les mathematiques en cryptographie
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tomcrypt.h>
#include <gmp.h>
#include <math.h>
#include <stdint.h>
#include <string.h>
#include "pgp.h"
#include "envoi.h"
#include "commandes.h"
#include "lire_ecrire.h"
#include "gestion_cles.h"
#include "math_crypto.h"
#include "types.h"

/** Fonction GMP pour initialiser les variables **/
void initialise_memoire(mpz_t p, mpz_t q, mpz_t n, mpz_t z, mpz_t e, mpz_t d)
{
	mpz_init(p);
	mpz_init(q);
	mpz_init(n);
	mpz_init(z);
	mpz_init(e);
	mpz_init(d);
}

/** Fonction GMP pour libérer la mémoire **/
void libere_memoire(mpz_t p, mpz_t q, mpz_t n, mpz_t z, mpz_t e, mpz_t d,
		    gmp_randstate_t state)
{
	mpz_clear(p);
	mpz_clear(q);
	mpz_clear(n);
	mpz_clear(z);
	mpz_clear(e);
	mpz_clear(d);
	gmp_randclear(state);
}

/** Fonction qui met dans p un nombre premier aléatoire **/
// p est le futur nombre premier, state correspond à l'aléatoire et choix correspond à la taille du nombre
void determine_premier(mpz_t p, gmp_randstate_t state, int choix)
{
	mpz_urandomb(p, state, choix);	//met dans p un nombre aleatoire
	int test = mpz_probab_prime_p(p, 10);	//renvoie 0 si c'est pas premier, 1 si c'est surement premier et 2 si c'est premier (sur)
	while (((test != 1) && (test != 2))) {	//continue de choisir un nombre tant qu'il n'est pas premier
		mpz_urandomb(p, state, choix);
		test = mpz_probab_prime_p(p, 10);	//10 est une valeur correcte pour faire le test (voir manuel gmp)
	}
}

/** Fonction qui met dans n le produit de p et q **/
void determine_n(mpz_t p, mpz_t q, mpz_t n)
{
	mpz_addmul(n, p, q);
}

/** Fonction qui met dans n le produit de p-1 et q-1 **/
void determine_z(mpz_t p, mpz_t q, mpz_t z)
{
	mpz_t pp, ppp, qq, qqq;
	mpz_init(ppp);
	mpz_init(qqq);
	mpz_init_set(pp, p);	//copie p dans pp
	mpz_init_set(qq, q);	//copie q dans qq
	mpz_sub_ui(ppp, pp, 1);	//ppp=pp-1
	mpz_sub_ui(qqq, qq, 1);	//qqq=qq-1
	mpz_addmul(z, ppp, qqq);	//z=ppp*qqq
	mpz_clear(pp);
	mpz_clear(ppp);
	mpz_clear(qq);
	mpz_clear(qqq);
}

/** Fonction qui calcule e tq pgcd(e,z)=1 (calcul euclide etendu) **/
void determine_e(mpz_t z, gmp_randstate_t state, mpz_t e)
{
	mpz_t pgcd;		//variable qui contiendra le pgcd
	mpz_init(pgcd);
	int test;
	do {
		mpz_urandomb(e, state, TAILLE);	//choisit un nb aleatoire
		mpz_gcd(pgcd, e, z);	//CALCUL PGCD(e,z);
		test = mpz_cmp_d(pgcd, 1);	//PGCD=1?
	} while (test != 0);	//tant que pgcd n'est pas égal a 1 on continue
	mpz_clear(pgcd);
}

/** Fonction qui met dans d la valeur de l'inverse de e modulo z **/
void determine_d(mpz_t p, mpz_t q, mpz_t n, mpz_t z, mpz_t e, mpz_t d,
		 gmp_randstate_t state)
{
	int calc = mpz_invert(d, e, z);	//met dans d l'inverse multiplicatif de e mod z
	if (calc == 0) {	//si c'est pas possible on quitte
		libere_memoire(p, q, n, z, e, d, state);
		printf("CLES INDETERMINEES\n");
		exit(1);
	}
}

/** Fonction qui renvoie un entier aleatoirement (pseudo aleatoire) **/
int random_int(gmp_randstate_t state)
{
	mpz_t n;
	mpz_init(n);
	mpz_urandomb(n, state, 7);
	double i;
	i = mpz_get_d(n);
	if ((i <= 31) || (i >= 127))
		i = random_int(state);
	mpz_clear(n);
	int ii = i;
	return ii;
}

/** Fonction qui calcule le XOR de deux entiers **/
int xor(int i, int j)
{
	return i ^ j;
}

/** Fonction qui convertit un entier en chaine de caracteres **/
void myitoa(int in, char *out, int longueur)
{
	int i, quotient, reste;
	int j = longueur;
	int compteur = 0;
	for (i = 0; i < j - 1; i++) {
		out[i] = '0';
	}
	out[j - 1] = '\0';
	while (in != 0) {
		quotient = in / 2;
		reste = in % 2;
		if (reste == 0)
			out[j - 2 - compteur] = '0';
		else
			out[j - 2 - compteur] = '1';
		compteur++;
		in = quotient;
	}
}

/** Fonction f pour la creation du hash SHA-1 **/
uint32_t f(uint32_t B, uint32_t C, uint32_t D, int t)
{
	uint32_t f;
	if ((0 <= t) && (t <= 19))
		f = ((B & C) | ((~B) & D));	//F1
	else if ((20 <= t) && (t <= 39))
		f = B ^ C ^ D;	//F2
	else if ((40 <= t) && (t <= 59))
		f = ((B & C) | (B & D) | (C & D));	//F3
	else if ((60 <= t) && (t <= 79))
		f = B ^ C ^ D;	//F4
	else
		f = -1;
	return f;
}

/** Fonction k pour la creation du hash SHA-1 **/
uint32_t k(int t)
{
	uint32_t out;
	if ((0 <= t) && (t <= 19))
		out = K1;
	else if ((20 <= t) && (t <= 39))
		out = K2;
	else if ((40 <= t) && (t <= 59))
		out = K3;
	else if ((60 <= t) && (t <= 79))
		out = K4;
	else
		out = -1;
	return out;
}

/** Fonction de shift utile pour le hash SHA-1 **/
uint32_t shift(int n, uint32_t X)
{
	uint32_t out = ((X << n) | (X >> (32 - n)));
	return out;
}

/** Fonction qui convertit en binaire une chaine de caracteres **/
int converbi(char *chaine)
{				//TAILLE 4
	int out = 0;
	double rang = 3;
	int caset = 0;
	while (rang != -1) {
		if (chaine[caset] == '1') {
			out = out + pow(2, rang);
		}
		rang--;
		caset++;
	}
	return out;
}

/** Fonction qui convertit en hexadecimal une chaine de caracteres **/
uint32_t converhexa(char *chaine)
{				//TAILLE 9
	uint32_t out = 0;
	uint32_t puiss;
	double rang = 8 - 1;
	int caset = 0;
	while (rang != -1) {
		puiss = pow(16, rang);
		if (chaine[caset] == '1') {
			out = out + puiss;
		} else if (chaine[caset] == '2') {
			out = out + 2 * puiss;
		} else if (chaine[caset] == '3') {
			out = out + 3 * puiss;
		} else if (chaine[caset] == '4') {
			out = out + 4 * puiss;
		} else if (chaine[caset] == '5') {
			out = out + 5 * puiss;
		} else if (chaine[caset] == '6') {
			out = out + 6 * puiss;
		} else if (chaine[caset] == '7') {
			out = out + 7 * puiss;
		} else if (chaine[caset] == '8') {
			out = out + 8 * puiss;
		} else if (chaine[caset] == '9') {
			out = out + 9 * puiss;
		} else if (chaine[caset] == 'a') {
			out = out + 10 * puiss;
		} else if (chaine[caset] == 'b') {
			out = out + 11 * puiss;
		} else if (chaine[caset] == 'c') {
			out = out + 12 * puiss;
		} else if (chaine[caset] == 'd') {
			out = out + 13 * puiss;
		} else if (chaine[caset] == 'e') {
			out = out + 14 * puiss;
		} else if (chaine[caset] == 'f') {
			out = out + 15 * puiss;
		}
		rang--;
		caset++;
	}
	return out;
}

/** Fonction qui remplit une chaine de caracteres de 0 **/
void remplit_zeros(char *t, int taille)
{
	int i;
	for (i = 0; i < taille; i++) {
		t[i] = '0';
	}
	t[taille - 1] = '\0';
}

/** Fonction qui fait t[rang]='val' **/
void remplit_valeur(char *t, int rang, int val)
{
	if (val == 0)
		t[rang] = '0';
	else if (val == 1)
		t[rang] = '1';
	else if (val == 2)
		t[rang] = '2';
	else if (val == 3)
		t[rang] = '3';
	else if (val == 4)
		t[rang] = '4';
	else if (val == 5)
		t[rang] = '5';
	else if (val == 6)
		t[rang] = '6';
	else if (val == 7)
		t[rang] = '7';
	else if (val == 8)
		t[rang] = '8';
	else if (val == 9)
		t[rang] = '9';
	else if (val == 10)
		t[rang] = 'a';
	else if (val == 11)
		t[rang] = 'b';
	else if (val == 12)
		t[rang] = 'c';
	else if (val == 13)
		t[rang] = 'd';
	else if (val == 14)
		t[rang] = 'e';
	else if (val == 15)
		t[rang] = 'f';
}

/** Fonction qui affiche une chaine de caracteres **/
void affiche(char *message, int longueur)
{
	int i;
	for (i = 0; i < longueur; i++) {
		if ((i % 8 == 7))
			printf("%c ", message[i]);
		else
			printf("%c", message[i]);
		if (i % 32 == 31)
			printf("\n");
	}
	printf("\n");
}

/** Fonction d'initialisation des registres lors de la creation du hash SHA-1 **/
SHA1 init_registres(SHA1 hash)
{
	hash.registre2[0] = 0x67452301;
	hash.registre2[1] = 0xEFCDAB89;
	hash.registre2[2] = 0x98BADCFE;
	hash.registre2[3] = 0x10325476;
	hash.registre2[4] = 0xC3D2E1F0;
	//H0,H1,H2,H3,H4,H5 initialises
	return hash;
}

/** Fonction d'initialisation 16W pour la creation du hash SHA-1 **/
SHA1 init_16W(SHA1 hash, int rang)
{
	int i;
	for (i = 0; i < 16; i++) {
		hash.W[i] = hash.mot[i][rang];
	}
	return hash;
}

/** Fonction d'initialisation generale du hash SHA-1 **/
SHA1 init_sha1(char *message)
{
	SHA1 hash;
	hash = init_registres(hash);
	int taille = strlen(message);
	int nb = taille * sizeof(char) * TAILLE_OCTET + 1 + 1;	//POUR 1 EN PLUS
	char binaire[nb];	//contiendra le binaire du message
	binaire[0] = '\0';
	char lettre[sizeof(char) * TAILLE_OCTET + 1];	//contiendra le binaire de la lettre
	unsigned char c;
	int j;
	for (j = 0; j < strlen(message); j++) {
		c = message[j];
		myitoa(c, lettre, sizeof(lettre));
		strcat(binaire, lettre);
	}
	char temp[2];
	temp[0] = '1';
	temp[1] = '\0';
	strcat(binaire, temp);

	int calc = 0;
	int k;
	for (k = 0; k < 512; k++) {
		if ((strlen(binaire) + 64 + k) % 512 == 0) {
			calc = k;
		}
	}
	hash.nbBlocs = (strlen(binaire) + 64 + calc) / 512;

	char blocZero[calc + 1];
	remplit_zeros(blocZero, calc + 1);
	char finalBinaire[strlen(binaire) + 64 + calc + 1];
	finalBinaire[0] = '\0';
	strcat(finalBinaire, binaire);
	strcat(finalBinaire, blocZero);

	char blocTaille[64 + 1];
	remplit_zeros(blocTaille, sizeof(blocTaille));
	myitoa(taille * TAILLE_OCTET, blocTaille, sizeof(blocTaille));
	strcat(finalBinaire, blocTaille);

	char finalHexa[strlen(finalBinaire) / 4 + 1];
	finalHexa[sizeof(finalHexa) - 1] = '\0';
	int grQuatre;
	int p = 0;
	int rang = 0;
	char paquet[5];
	paquet[4] = '\0';
	do {
		paquet[0] = finalBinaire[p];
		paquet[1] = finalBinaire[p + 1];
		paquet[2] = finalBinaire[p + 2];
		paquet[3] = finalBinaire[p + 3];
		grQuatre = converbi(paquet);
		remplit_valeur(finalHexa, rang, grQuatre);
		rang++;
		p = p + 4;
	} while (p != strlen(finalBinaire));
	int d;
	for (d = 0; d < 16; d++) {
		hash.mot[d] = malloc(hash.nbBlocs * sizeof(uint32_t));
	}
	p = 0;
	rang = 0;
	uint32_t grHuit;
	char paquet1[9];
	paquet1[8] = '\0';
	do {
		paquet1[0] = finalHexa[p];
		paquet1[1] = finalHexa[p + 1];
		paquet1[2] = finalHexa[p + 2];
		paquet1[3] = finalHexa[p + 3];
		paquet1[4] = finalHexa[p + 4];
		paquet1[5] = finalHexa[p + 5];
		paquet1[6] = finalHexa[p + 6];
		paquet1[7] = finalHexa[p + 7];
		grHuit = converhexa(paquet1);
		hash.mot[(p / 8) % 16][p / (16 * 8)] = grHuit;
		rang++;
		p = p + 8;
	} while (p != strlen(finalHexa));
	return hash;
}

/** Fonction de changements binaires du hash SHA-1 **/
void process_sha1(SHA1 hash, char *out)
{
	int i, t;
	uint32_t TEMP;
	for (i = 0; i < hash.nbBlocs; i++) {
		hash = init_16W(hash, i);
		for (t = 16; t < 80; t++) {
			hash.W[t] =
			    (shift
			     (1,
			      (hash.W[t - 3] ^ hash.W[t - 8] ^ hash.
			       W[t - 14] ^ hash.W[t - 16])));
		}
		hash.registre1[0] = hash.registre2[0];	//A<-HO
		hash.registre1[1] = hash.registre2[1];	//B<-H1
		hash.registre1[2] = hash.registre2[2];	//C<-H2
		hash.registre1[3] = hash.registre2[3];	//D<-H3
		hash.registre1[4] = hash.registre2[4];	//E<-H4

		for (t = 0; t < 80; t++) {
			TEMP =
			    (shift(5, hash.registre1[0])) + f(hash.registre1[1],
							      hash.registre1[2],
							      hash.registre1[3],
							      t) +
			    hash.registre1[4] + hash.W[t] + k(t);
			hash.registre1[4] = hash.registre1[3];
			hash.registre1[3] = hash.registre1[2];
			hash.registre1[2] = shift(30, hash.registre1[1]);
			hash.registre1[1] = hash.registre1[0];
			hash.registre1[0] = TEMP;
		}

		hash.registre2[0] = hash.registre2[0] + hash.registre1[0];
		hash.registre2[1] = hash.registre2[1] + hash.registre1[1];
		hash.registre2[2] = hash.registre2[2] + hash.registre1[2];
		hash.registre2[3] = hash.registre2[3] + hash.registre1[3];
		hash.registre2[4] = hash.registre2[4] + hash.registre1[4];
	}

	sprintf(out, "%0x%0x%0x%0x%0x", hash.registre2[0], hash.registre2[1],
		hash.registre2[2], hash.registre2[3], hash.registre2[4]);
}

/** Fonction pour libérer la memoire du hash SHA-1 **/
void done_sha1(SHA1 hash)
{
	int i;
	for (i = 0; i < 16; i++) {
		free(hash.mot[i]);
	}
}
