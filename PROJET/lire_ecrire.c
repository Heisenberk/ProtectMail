/**
 * \file lire_ecrire.c
 * \author Claire Baskevitch - Clément Caumes
 * \date 2017
 * \brief contient les fonctions concernant les entrees/sorties
 */

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <gmp.h>
#include "pgp.h"
#include "lire_ecrire.h"
#include "commandes.h"
#include "gestion_cles.h"
#include "math_crypto.h"
#include "types.h"

/** Affiche la date et l'heure **/
void affiche_date_heure()
{
	time_t now = time(NULL);
	struct tm tm_now = *localtime(&now);
	char s_now[sizeof "JJ/MM/AAAA HH:MM:SS"];
	strftime(s_now, sizeof s_now, "%d/%m/%Y %H:%M:%S", &tm_now);
	printf("%s\n", s_now);
}

/** Affiche la présentation générale de PGP **/
void affiche_general()
{
	printf
	    ("\033[34m\033[01m\nSimplified Privacy Guard - Hybrid Cryptography\n");
	printf
	    ("Université Versailles Saint Quentin en Yvelines - 2017\nClaire Baskevitch - Clément Caumes\n");
	printf("Date courante : ");
	affiche_date_heure();
	printf("\n\033[0m");
}

/** Affiche les commande : ./pgp -h **/
void affiche_commandes()
{
	printf
	    ("\033[01m- Pour chiffrer un fichier:\033[32m ./pgp -c [fichier]\033[37m Le fichier message chiffré sera dans \033[31m[fichier.pgp]\n\033[0m");
	printf
	    ("\033[01m- Pour déchiffrer un fichier:\033[32m ./pgp -w [fichier.pgp]\033[37m Le contenu déchiffré du fichier s'affichera à l'écran\n\033[0m");
	printf
	    ("\033[01m- Pour déchiffrer un fichier:\033[32m ./pgp [fichier.pgp]\033[37m Le fichier message déchiffré sera dans \033[31m[fichier]\n\033[0m");
	printf
	    ("\033[01m- Pour signer un message:\033[32m ./pgp -s [fichier]\n\033[0m");
	printf
	    ("\033[01m- Pour vérifier l'authentification de l'auteur d'un message: \033[32m ./pgp -v \033[31m[fichier.pgp]\n\033[0m");
	printf
	    ("\033[01m- Pour générer un couple de clés privées/publiques:\033[32m ./pgp -kg \n\033[0m");
	printf("\n\033[0m");
}

/** Retourne la taille des clés voulu par l'utilisateur **/
int demande_taille_cles()
{
	char choix[16];
	int i;
	printf("\033[01mChoisissez la taille de votre clé RSA :\n");
	printf
	    ("	1)	512 bits - Low commercial grade, fast but less secure\n");
	printf
	    ("	2)	768 bits - High commercial grade, medium speed, good security\n");
	printf
	    ("	3)	1024 bits - Military grade, slow, highest security\n");
	printf
	    ("	4)	2048 bits - Restricted by the E.U & U.S governments\n");
	printf("Choisissez 1, 2, 3 ou 4 : ");
	scanf("%s", choix);
	if (strlen(choix) >= 2)
		quitte_demande_invalide();
	if (!isdigit(choix[0]))
		quitte_demande_invalide();
	if ((atoi(choix) == 0) || (atoi(choix) >= 5)) {
		quitte_demande_invalide();
	}
	if (atoi(choix) == 1)
		i = 512;
	else if (atoi(choix) == 2)
		i = 768;
	else if (atoi(choix) == 3)
		i = 1024;
	else
		i = 2048;
	printf("\033[01mVous avez choisi une clé de %d bits.\n\n\033[0m", i);
	return i;
}

/** Renvoie la concatenation de s avec ".pgp" **/
char *concatenation_extension(char *s)
{
	char *concat =
	    malloc(strlen(s) * sizeof(char) + strlen(".pgp") * sizeof(char) +
		   sizeof(char));
	char *final = strcat(strcpy(concat, s), ".pgp");
	return final;
}

/** Fonction qui met la cle publique du destinataire demandé dans les parametres **/
void cherche_cle_pub(mpz_t n, mpz_t e)
{
	printf
	    ("\033[01mEcrire le nom de la clé publique de votre destinataire: \033[33m");
	char nom[256];
	scanf("%s", nom);
	FILE *f = fopen(nom, "r");
	if (f == NULL) {
		printf("\033[37mCette clé n'existe pas.\n\n\033[0m");
		exit(1);
	} else {
		printf("\033[0m");
		char c;
		int compteur_lignes = 0;
		while (compteur_lignes < 3) {
			c = fgetc(f);
			if (c == '\n')
				compteur_lignes++;
		}
		gmp_fscanf(f, "%Zd\n%Zd", n, e);	//scanne les 2 nombres n et e
		fclose(f);
	}
}

/** Fonction qui met notre cle privee demandé dans les parametres **/
void cherche_cle_priv(mpz_t n, mpz_t d)
{
	printf("\033[01mEcrire le nom de votre clé privée: \033[33m");
	char nom[256];
	scanf("%s", nom);
	FILE *f = fopen(nom, "r");
	if (f == NULL) {
		printf("\033[37mCette clé n'existe pas.\n\n\033[0m");
		exit(1);
	} else {
		printf("\033[0m");
		char c;
		int compteur_lignes = 0;
		while (compteur_lignes < 1) {
			c = fgetc(f);
			if (c == '\n')
				compteur_lignes++;
		}
		gmp_fscanf(f, "%Zd\n%Zd", n, d);	//scanne les 2 nombres n et e
		fclose(f);
	}
}

/* Ecriture sur un fichier ou sur le terminal */

/** Ecriture fichier 1 **/
void ecrit_bordure_sup_rsa_pub(FILE * f)
{
	fprintf(f, "-----BEGIN PGP PUBLIC KEY BLOCK-----\n");
}

/** Ecriture fichier 2 **/
void ecrit_bordure_sup_rsa_priv(FILE * f)
{
	fprintf(f, "-----BEGIN PGP PRIVATE KEY BLOCK-----\n");
}

/** Ecriture fichier 3 **/
void ecrit_bordure_sup_m_sig(FILE * f)
{
	fprintf(f, "-----BEGIN PGP SIGNED MESSAGE-----\n");
}

/** Ecriture fichier 4 **/
void ecrit_bordure_sup_m_chiffre(FILE * f)
{
	fprintf(f, "-----BEGIN PGP MESSAGE-----\n");
}

/** Ecriture fichier 5 **/
void ecrit_bordure_sup_id(FILE * f, char *s1, char *s2, char *s3)
{
	fprintf(f, "-----IDENTITY-----\n");
	fprintf(f, "%s %s %s\n", s1, s2, s3);
}

/** Ecriture fichier 6 **/
void ecrit_bordure_inf_m_sig(FILE * f)
{
	fprintf(f, "\n-----BEGIN PGP SIGNATURE-----\n");
}

/** Ecriture fichier 7 **/
void ecrit_bordure_inf_m_chiffre(FILE * f)
{
	fprintf(f, "-----END PGP MESSAGE-----\n");
}

/** Ecriture fichier 8 **/
void ecrit_bordure_inf_rsa_pub(FILE * f)
{
	fprintf(f, "-----END PGP PUBLIC KEY BLOCK-----\n");
}

/** Ecriture fichier 9 **/
void ecrit_bordure_inf_rsa_priv(FILE * f)
{
	fprintf(f, "-----END PGP PRIVATE KEY BLOCK-----\n");
}

/** Ecriture fichier 10 **/
void ecrit_bordure_inf_m_sig_final(FILE * f)
{
	fprintf(f, "-----END PGP SIGNED MESSAGE-----\n");
}

/** Ecriture terminal 1 **/
void affiche_action_pgp(char *nom)
{
	printf
	    ("\033[01mLe fichier à envoyer est sous le nom : \033[31m%s\n\n\033[0m",
	     nom);
}

/** Ecriture terminal 2 **/
void affiche_dechiffrement(char *nomFichier)
{
	printf("AFFICHAGE FICHIER DECHIFFRE\n\n");
}

/** Ecriture terminal 3 **/
void affiche_fichier_dechiffre(char *nomFichier)
{
	printf
	    ("\033[01mLe contenu déchiffré du fichier \033[31m%s\033[37m est : \n\n\033[0m",
	     nomFichier);
	affiche_dechiffrement(nomFichier);
}

/*------------------------------------------*/

/** Ecrit le contenu de "origin" dans "new" **/
void recopie_message(FILE * origin, FILE * new)
{
	char c = 'a';
	while (c != EOF) {
		c = fgetc(origin);
		if (c != EOF) {
			fprintf(new, "%c", c);
		}
	}
}

/** Affiche le contenu d'un fichier **/
void affiche_contenu_fic(char *nomFichier)
{
	FILE *f = fopen(nomFichier, "r");
	char c;
	do {
		c = fgetc(f);
		if (c != EOF)
			printf("\033[34m%c", c);
	} while (c != EOF);
	printf("\n\033[0m");
	fclose(f);
}

/** Renvoie la réponse de l'utilisateur OUI/NON **/
int teste_reponse(char *s)
{
	int o1 = strcmp(s, "oui");
	int n1 = strcmp(s, "non");
	int o2 = strcmp(s, "Oui");
	int n2 = strcmp(s, "Oui");
	int o3 = strcmp(s, "OUI");
	int n3 = strcmp(s, "OUI");
	int o4 = strcmp(s, "OuI");
	int n4 = strcmp(s, "OuI");
	int o5 = strcmp(s, "oUi");
	int n5 = strcmp(s, "oUi");
	int o6 = strcmp(s, "oUI");
	int n6 = strcmp(s, "oUI");
	int o7 = strcmp(s, "O");
	int n7 = strcmp(s, "N");
	int o8 = strcmp(s, "o");
	int n8 = strcmp(s, "n");
	if ((!o1) || (!o2) || (!o3) || (!o4) || (!o5) || (!o6) || (!o7)
	    || (!o8)) {
		return 1;
	}
	if ((!n1) || (!n2) || (!n3) || (!n4) || (!n5) || (!n6) || (!n7)
	    || (!n8)) {
		return 2;
	} else
		return -1;
}

/** Procédure qui affiche le contenu d'un fichier si nécessaire **/
void demande_visualisation_message(char *message)
{
	printf
	    ("\033[01mFichier corrompu. Voulez-vous quand même l'afficher?(o/N) ");
	char buffer[256];
	scanf("%s", buffer);
	if (teste_reponse(buffer) == 1) {
		printf("\n\033[34m%s\n\n\033[0m", message);
	} else
		quitte_pas_probleme();
}

/** Ecrit la clé privée dans le fichier secring.pgp **/
void ecrit_cle_privee(mpz_t n, mpz_t d)
{
	FILE *f = fopen("secring.pgp", "w");
	ecrit_bordure_sup_rsa_priv(f);
	char pass_phrase[256];
	cree_pass_phrase(pass_phrase);	//Pass phrase normalement utilisee pour chiffrer le fichier
	gmp_fprintf(f, "%Zd\n", n);
	gmp_fprintf(f, "%Zd\n", d);
	ecrit_bordure_inf_rsa_priv(f);
	fclose(f);
}

/** Ecrit la clé publique dans le fichier pubring.pgp **/
void ecrit_cle_publique(char *s1, char *s2, char *s3, mpz_t n, mpz_t e)
{
	FILE *f = fopen("pubring.pgp", "w");
	ecrit_bordure_sup_id(f, s1, s2, s3);
	ecrit_bordure_sup_rsa_pub(f);

	gmp_fprintf(f, "%Zd\n", n);
	gmp_fprintf(f, "%Zd\n", e);

	ecrit_bordure_inf_rsa_pub(f);
	fclose(f);
	printf("\n");
}

/** Fonction qui compte le nombre de caracteres dans un fichier **/
int compte_nb_car_fichier(FILE * f)
{
	int compteur = 0;
	char c = 'a';
	while (c != EOF) {
		c = fgetc(f);
		compteur++;
	}
	fseek(f, 0, SEEK_SET);
	return compteur - 1;
}

/** Fonction qui compte le nombre de caracteres dans un fichier.pgp avec signature **/
int compte_nb_car_fichier_signature(FILE * f)
{
	int compteur_retour_ligne = 0;
	char c = 'a';
	while (c != EOF) {	//on compte le nombre de lignes
		c = fgetc(f);
		if (c == '\n')
			compteur_retour_ligne++;
	}
	fseek(f, 0, SEEK_SET);	//on revient au debut
	int compteur_lignes = compteur_retour_ligne - 44;	//44 nb de lignes min pr la signature
	//on a calculé le nombre de lignes du message
	int compteur_caracteres = -1;
	int i = 0;
	c = 'a';
	while (c != '\n')
		c = fgetc(f);	//on va au debut du message
	while (i < compteur_lignes) {
		c = fgetc(f);
		compteur_caracteres++;
		if (c == '\n')
			i++;
	}
	fseek(f, 0, SEEK_SET);
	return compteur_caracteres;
}

/** Fonction qui met les caracteres du fichier dans message **/
void remplit_chaine_carac_message(FILE * f, char *message, int taille)
{
	message[taille - 1] = '\0';
	int i;
	for (i = 0; i < taille - 1; i++) {
		message[i] = fgetc(f);
	}
	fseek(f, 0, SEEK_SET);
}

/** Fonction qui met les caracteres du fichier.pgp avec signature dans message **/
void remplit_chaine_carac_message_signe(FILE * f, char *message, int taille)
{
	char c = 'a';
	while (c != '\n')
		c = fgetc(f);
	message[taille - 1] = '\0';
	int i;
	for (i = 0; i < taille - 1; i++) {
		message[i] = fgetc(f);
	}
	c = fgetc(f);
	c = fgetc(f);
}

/** Fonction qui teste si le fichier f est bien un fichier.pgp avec signature **/
int teste_fichier_signature(FILE * f)
{
	char message[35] = "-----BEGIN PGP SIGNED MESSAGE-----";
	char lecture[35];
	lecture[34] = '\0';
	int i;
	for (i = 0; i < 34; i++) {
		lecture[i] = fgetc(f);
	}
	for (i = 0; i < 35; i++) {
		if (lecture[i] != message[i])
			return 0;
	}
	return 1;
}
