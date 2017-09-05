//VERIFIE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tomcrypt.h>
#include <gmp.h>
#include "pgp.h"
#include "envoi.h"
#include "commandes.h"
#include "lire_ecrire.h"
#include "gestion_cles.h"
#include "math_crypto.h"
#include "types.h"

/*hash_state md;
	unsigned char* in="bakcms";
	unsigned char out[16];
	md5_init(&md);
	md5_process(&md,in,strlen(in));
	md5_done(&md,out);

int teste_pass_phrase(){
	printf("\033[01mEntrer la Pass Phrase:\033[0m");
	char* buffer;
	scanf("%s\n",&buffer);
	printf(">%s\n",buffer);
	
}*/

// Renvoie la concatenation de s avec ".pgp"
char* concatenation_extension(char* s){
	char* concat=malloc(strlen(s)*sizeof(char)+strlen(".pgp")*sizeof(char)+sizeof(char));
	char* final=strcat(strcpy(concat,s),".pgp");
	return final;
}

void ecrit_message_non_chiffre(int num,char**chaineCarac){
	char* nom=concatenation_extension(chaineCarac[2]);
	FILE* origin=fopen(chaineCarac[2],"r");
	FILE* new=fopen(nom,"w");
	ecrit_bordure_sup_m_sig(new);
	recopie_message(origin,new);
	ecrit_bordure_inf_m_sig(new);
	fclose(origin);
	fclose(new);
	affiche_action_pgp(nom);
	free(nom);
}

void cree_fichier_chiffre(char* nomFichier){
	mpz_t n,e; //clé publique du destinataire
	mpz_init(n);
	mpz_init(e);
	cherche_cle_pub(n,e);
	char* nom=concatenation_extension(nomFichier);
	FILE* origin=fopen(nomFichier,"r");
	FILE* new=fopen(nom,"w");
	ecrit_bordure_sup_m_chiffre(new);
	CLE newsession=genere_cle_session(); //genere cle de session aleatoire
	encrypt_rsa_chaine(newsession.session,new,n,e); //chiffrement rsa de la cle de session
	fclose(origin);
	encrypt_session(nomFichier,new,newsession); //chiffrement XOR du message
	ecrit_bordure_inf_m_chiffre(new);
	fclose(new);
	affiche_action_pgp(nom);
	free(nom);
	mpz_clear(n);
	mpz_clear(e);

}

