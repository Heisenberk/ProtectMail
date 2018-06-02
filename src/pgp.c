/**
 * \file pgp.c
 * \author Claire Baskevitch - Clément Caumes
 * \date 2017
 * \brief contient le main du projet
 */

#include <stdio.h>
#include <stdlib.h>
#include "pgp.h"
#include "envoi.h"
#include "reception.h"
#include "commandes.h"
#include "lire_ecrire.h"
#include "gestion_cles.h"
#include "math_crypto.h"
#include "types.h"

int main(int argc, char **argv)
{
	int mode = teste_commande_general(argc, argv);
	/** envoyer un message non chiffré et signé **/
	if (mode == MODE_SIGN_NN_CHIFFRE) {
		ecrit_message_non_chiffre_signe(argc, argv);
	}
	/** verifier l'authentification d'un message non chiffre et signe **/
	else if (mode == MODE_SIGN_CHIFFRE) {
		verifie_authentification(argv[2]);
	} else if (mode == MODE_CHIFFREMENT) {
		cree_fichier_chiffre(argv[2]);
	}
	/** recevoir un message chiffré **/
	else if (mode == MODE_DECHIFFREMENT) {
		cree_fichier_dechiffre(argv[1]);
	} else if (mode == MODE_AFFICHAGE_DECHIFFRE) {
		ecrit_message_dechiffre(argv[2]);
	}
	/** genere nouvelles cles rsa **/
	else if (mode == MODE_GENERATION_CLES) {
		genere_cles();
	}

	return 0;
}
