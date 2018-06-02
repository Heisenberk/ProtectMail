/**
 * \file gestion_cles.c
 * \author Claire Baskevitch - Clément Caumes
 * \date 2017
 * \brief contient les fonctions concernant le chiffrement/déchiffrement et les cles 
 */

#include <stdio.h>
#include <tomcrypt.h>
#include <stdlib.h>
#include <gmp.h>
#include "pgp.h"
#include "gestion_cles.h"
#include "commandes.h"
#include "math_crypto.h"
#include "lire_ecrire.h"

/** Chiffre "nomFichier" grâce à la clé publique (e,n) **/
/*void encrypt_rsa_fic(char* nomFichier,mpz_t n,mpz_t e){
	FILE* f1=fopen(nomFichier,"r"); 
	if(f1==NULL) quitte_fichier_inexistant();
	FILE* f2=fopen("message_chiffre","w");
	if(f1==NULL){
		printf("FICHIER IMPOSSIBLE A CREER\n");
		exit(1);
	}
	char c;double ascii;mpz_t m,u;int test;
	int compteur=0; 
	mpz_init(m);mpz_init(u);
	do{
		c=fgetc(f1);
		ascii=c;
		if(c!=EOF){
			mpz_set_d(m,ascii); //met dans m --> ascii
			mpz_powm(u,m,e,n); //u=m^e mod n
			test=gmp_fprintf(f2,"%Zd ",u);
			if(test==-1) exit(1);
			compteur++;//
			if((compteur%5)==0)fprintf(f2,"\n");
		}
	}while(c!=EOF);
	fclose(f1);
	fclose(f2);
	mpz_clear(m);
	mpz_clear(u);
	printf("\nMESSAGE CHIFFRE\n");
}*/

/** Déchiffre "nomFichier" grâce à la clé privée (d,n) **/
/*void decrypt_rsa_fic(char* nomFichier,mpz_t n,mpz_t d){
	FILE* f1=fopen(nomFichier,"r");
	if(f1==NULL) quitte_fichier_inexistant();
	FILE* f2=fopen("message_dechiffre","w");
	if(f2==NULL){
		printf("FICHIER IMPOSSIBLE A CREER\n");
		exit(1);
	}
	char c;mpz_t m,u;int test;unsigned long int conv;int car;
	mpz_init(m);mpz_init(u);
	do{
		c=fgetc(f1);
		if((c!=EOF)&&(isdigit(c))){
			ungetc(c,f1);
			test=gmp_fscanf(f1,"%Zd",m);
			if(test==-1) exit(1);
			mpz_powm(u,m,d,n); //u=m^d mod n
			conv=mpz_get_ui(u); // car prend la valeur de u
			car=conv;
			fprintf(f2,"%c",car);
			while(c!=' ') c=fgetc(f1);
		}
	}while(c!=EOF);
	fclose(f1);
	fclose(f2);
	mpz_clear(m);
	mpz_clear(u);
	printf("\nMESSAGE DECHIFFRE\n");
}*/

/** Chiffrement RSA (cle publique) de la cle de session (chaine) **/
void encrypt_rsa_chaine(char *chaine, int taille, FILE * new, mpz_t n, mpz_t e)
{
	char c;
	double ascii;
	mpz_t m, u;
	int test;
	int compteur = 0;
	int i;
	mpz_init(m);
	mpz_init(u);
	for (i = 0; i < taille; i++) {
		c = chaine[i];
		ascii = c;
		mpz_set_d(m, ascii);	//met dans m --> ascii
		mpz_powm(u, m, e, n);	//u=m^e mod n
		test = gmp_fprintf(new, "%Zd\n", u);
		if (test == -1)
			exit(1);
		compteur++;
	}
	fprintf(new, "\n");
	mpz_clear(m);
	mpz_clear(u);
}

/** Dechiffrement RSA (cle privee) de la cle de session (chaine) **/
void decrypt_rsa_chaine(char *chaine, int taille, FILE * new, mpz_t n, mpz_t d)
{
	mpz_t h, u;
	int i;
	char c = 'a';
	while (c != '\n')
		c = fgetc(new);
	for (i = 0; i < taille; i++) {
		mpz_init(h);
		mpz_init(u);
		gmp_fscanf(new, "%Zd\n", h);
		mpz_powm(u, h, d, n);	//u=h^d mod n
		chaine[i] = mpz_get_d(u);
		mpz_clear(h);
		mpz_clear(u);
	}
}

////

/** Affichage de la cle privee **/
void genere_cle_privee(mpz_t n, mpz_t d)
{
	ecrit_cle_privee(n, d);
}

/** Affichage de la cle publique **/
void genere_cle_publique(mpz_t n, mpz_t e)
{
	char prenom[64];
	char nom[64];
	char adresse[64];
	printf
	    ("\033[01mVotre clé publique nécessite un prénom, un nom suivi de votre adresse mail fermée par les symboles < et > Par exemple : John Smith <1234.567@mail.com>\n");
	printf("Entrez votre prénom : \033[33m");
	scanf("%s", prenom);
	printf("\033[37mEntrez votre nom : \033[33m");
	scanf("%s", nom);
	printf("\033[37mEntrez votre mail entre crochets : \033[33m");
	scanf("%s", adresse);
	printf("\033[0m");
	ecrit_cle_publique(prenom, nom, adresse, n, e);
}

/** Génère les clés aléatoirement **/
void genere_cles()
{
	//TIMER : state permet de choisir SEED pour améliorer le pseudo aléatoire
	gmp_randstate_t state;
	gmp_randinit_default(state);
	gmp_randseed_ui(state, (unsigned)time(NULL));
	///////
	mpz_t p, q, n, z, e, d;
	int choix = demande_taille_cles();
	initialise_memoire(p, q, n, z, e, d);
	determine_premier(p, state, choix);
	determine_premier(q, state, choix);
	determine_n(p, q, n);
	determine_z(p, q, z);
	determine_e(z, state, e);
	determine_d(p, q, n, z, e, d, state);
	genere_cle_publique(n, e);
	genere_cle_privee(n, d);
	printf
	    ("\033[01m\033[31m\nGénération des clés publique et privée terminée\n\n\033[0m");
	libere_memoire(p, q, n, z, e, d, state);
}

/** Place dans buffer1 la Pass Phrase choisie par l'utilisateur **/
void cree_pass_phrase(char *buffer1)
{
	printf
	    ("\033[01mVous devez entrer une Pass Phrase pour protéger votre clé secrète RSA.\n");
	printf("\033[01mEntrez la Pass Phrase: \033[0m\033[30m");
	char buffer2[256];
	scanf("%s", buffer1);
	printf("\033[0m");
	printf("\033[01mEntrez de nouveau la Pass Phrase: \033[0m\033[30m");
	scanf("%s", buffer2);
	printf("\033[0m");
	if (strcmp(buffer1, buffer2)) {	//si ce n'est pas identique
		quitte_pass_phrase_incoherente();
	}
}

/** Generation d'une cle de session aleatoire **/
CLE genere_cle_session()
{
	//TIMER (pseudo aleatoire)
	gmp_randstate_t state;
	gmp_randinit_default(state);
	gmp_randseed_ui(state, (unsigned)time(NULL));
	//// 

	CLE new;
	new.session[16] = '\0';
	int i;
	for (i = 0; i < 16; i++) {
		new.session[i] = random_int(state);	//on met une valeur aleatoire
	}
	gmp_randclear(state);
	return new;
}

/** Chiffrement XOR de la cle de session avec les caracteres du fichier **/
void encrypt_session(char *nomFichier1, FILE * new, CLE clef)
{
	FILE *f1 = fopen(nomFichier1, "r");
	int k = 0;
	char c;
	do {
		c = fgetc(f1);
		if (c != EOF) {
			fprintf(new, "%d ", xor(clef.session[k % 16], c));
			k++;
		}
	} while (c != EOF);
	fprintf(new, "\n");
	fclose(f1);
}

/** Dechiffrement XOR de la cle de session avec les caracteres du fichier et ecriture dans fichier**/
void decrypt_session(CLE c, FILE * f_toDecrypt, FILE * f_vierge)
{
	int nb;
	int k = 0;
	char car = 'a';
	while (car != '\n') {
		fscanf(f_toDecrypt, "%d ", &nb);
		//printf("%d\n",nb);
		fprintf(f_vierge, "%c", xor(c.session[k % 16], nb));
		k++;
		car = fgetc(f_toDecrypt);
		if (car == '-')
			break;
		fseek(f_toDecrypt, -1, SEEK_CUR);

	}

}

/** dechiffrement XOR de la cle de session avec les caracteres du fichier et affichage **/
void decrypt_session_affichage(CLE c, FILE * f_toDecrypt)
{
	int nb;
	int k = 0;
	char car = 'a';
	printf("\033[01mLe contenu de ce fichier est le suivant: \n\n\033[33m");
	while (car != '\n') {
		fscanf(f_toDecrypt, "%d ", &nb);
		printf("%c", xor(c.session[k % 16], nb));
		k++;
		car = fgetc(f_toDecrypt);
		if (car == '-')
			break;
		fseek(f_toDecrypt, -1, SEEK_CUR);
	}
	printf("\n\033[0m");
}
