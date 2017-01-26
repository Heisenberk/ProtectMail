#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tomcrypt.h>
#include "pgp.h"
#include "envoi.h"
#include "commandes.h"
#include "lire_ecrire.h"
#include "gestion_cles.h"
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

/*FILE* cree_message_vide(int num,char**chaineCarac){
	char* nom=concatenation_extension(chaineCarac[2]);
	FILE* f=fopen(nom,"w");
	free(nom);
	fclose(f);
	return f;
}*/

// A DEPLACER 
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
	///////
	gmp_randstate_t state;
	gmp_randinit_default (state);
	gmp_randseed_ui(state,(unsigned)time(NULL));
	mpz_t p,q,n,z,e,d;
	int choix=demande_taille_cles();
	initialise_memoire(p,q,n,z,e,d);
	determine_premier(p,state,choix);
	determine_premier(q,state,choix);
	determine_n(p,q,n);
	determine_z(p,q,z);
	determine_e(z,state,e);
	determine_d(p,q,n,z,e,d,state);
	genere_cle_publique(n,e);
	genere_cle_privee(n,d);
	//////
	gmp_printf("%Zd %Zd\n",d,e);
	
	//LIRE ICI LES CLES DANS PUBRING.PGP!!!
	char* nom=concatenation_extension(nomFichier);
	FILE* origin=fopen(nomFichier,"r");
	FILE* new=fopen(nom,"w");
	ecrit_bordure_sup_m_chiffre(new);
	CLE newsession=genere_cle_session();
	encrypt_rsa_chaine(newsession.session,new,n,e);
	fclose(origin);
	encrypt_session(nomFichier,new,newsession);
	ecrit_bordure_inf_m_chiffre(new);
	//printf("\033[0m");
	//fclose(origin);
	fclose(new);
	free(nom);
	libere_memoire(p,q,n,z,e,d,state); //A ENLEVER

}

