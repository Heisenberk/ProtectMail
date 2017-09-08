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

void ecrit_message_non_chiffre_signe(int num,char**chaineCarac){
		
	char* nom=concatenation_extension(chaineCarac[2]);
	FILE* origin=fopen(chaineCarac[2],"r");
	int car=compte_nb_car_fichier(origin)+1;
	char* message=malloc(car*sizeof(char));
	remplit_chaine_carac_message(origin,message,car);
	//printf(">>%s<<\n",message);
	SHA1 hash; //le hash SHA1 est cree
	char final[TAILLE_HASH_SHA1];
	hash=init_sha1(message);
	process_sha1(hash,final);
	done_sha1(hash);
	
	mpz_t n,d;
	mpz_init(n); mpz_init(d);
	cherche_cle_priv(n,d); //la cle privee est stockee
	
	FILE* new=fopen(nom,"w");
	ecrit_bordure_sup_m_sig(new);
	recopie_message(origin,new);
	ecrit_bordure_inf_m_sig(new);
	encrypt_rsa_chaine(final,TAILLE_HASH_SHA1,new,n,d);
	ecrit_bordure_inf_m_sig_final(new);
	fclose(origin);
	fclose(new);
	affiche_action_pgp(nom);
	free(nom);
	free(message);
	mpz_clear(n);
	mpz_clear(d);
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
	encrypt_rsa_chaine(newsession.session,TAILLE_CLE_SESSION,new,n,e); //chiffrement rsa de la cle de session
	fclose(origin);
	encrypt_session(nomFichier,new,newsession); //chiffrement XOR du message
	ecrit_bordure_inf_m_chiffre(new);
	fclose(new);
	affiche_action_pgp(nom);
	free(nom);
	mpz_clear(n);
	mpz_clear(e);

}

