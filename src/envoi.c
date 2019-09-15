/**
 * \file envoi.c
 * \author Claire Baskevitch - Clément Caumes
 * \date 2017
 * \brief contient les fonctions concernant l'envoi de messages
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <tomcrypt.h>
#include <gmp.h>
#include "pgp.h"
#include "envoi.h"
#include "commandes.h"
#include "lire_ecrire.h"
#include "gestion_cles.h"
#include "math_crypto.h"
#include "types.h"

/** Fonction qui ecrit un nouveau message avec sa signature  **/
void ecrit_message_non_chiffre_signe(int num, char **chaineCarac)
{

	char *nom = concatenation_extension(chaineCarac[2]);
	FILE *origin = fopen(chaineCarac[2], "r");
	int car = compte_nb_car_fichier(origin) + 1;
	char *message = malloc(car * sizeof(char));
	remplit_chaine_carac_message(origin, message, car);
	SHA1 hash;		//le hash SHA1 est cree
	char final[TAILLE_HASH_SHA1];
	hash = init_sha1(message);
	process_sha1(hash, final);
	done_sha1(hash);

	mpz_t n, d;
	mpz_init(n);
	mpz_init(d);
	cherche_cle_priv(n, d);	//la cle privee est stockee

	FILE *new = fopen(nom, "w");
	ecrit_bordure_sup_m_sig(new);
	recopie_message(origin, new);
	ecrit_bordure_inf_m_sig(new);
	encrypt_rsa_chaine(final, TAILLE_HASH_SHA1, new, n, d);	//chiffrement rsa du hash
	ecrit_bordure_inf_m_sig_final(new);
	fclose(origin);
	fclose(new);
	affiche_action_pgp(nom);
	free(nom);
	free(message);
	mpz_clear(n);
	mpz_clear(d);
}

/** Fonction qui ecrit un nouveau message chiffre (XOR avec une cle de session) et chiffrement RSA de cette cle  **/
void cree_fichier_chiffre(char *nomFichier)
{
	mpz_t n, e;
	mpz_init(n);
	mpz_init(e);
	cherche_cle_pub(n, e);	//clé publique du destinataire
	char *nom = concatenation_extension(nomFichier);
	FILE *origin = fopen(nomFichier, "r");
	FILE *new = fopen(nom, "w");
	ecrit_bordure_sup_m_chiffre(new);
	CLE newsession = genere_cle_session();	//genere cle de session aleatoire
	encrypt_rsa_chaine(newsession.session, TAILLE_CLE_SESSION, new, n, e);	//chiffrement rsa de la cle de session
	fclose(origin);
	encrypt_session(nomFichier, new, newsession);	//chiffrement XOR du message
	ecrit_bordure_inf_m_chiffre(new);
	fclose(new);
	affiche_action_pgp(nom);
	free(nom);
	mpz_clear(n);
	mpz_clear(e);
}
