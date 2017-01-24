#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tomcrypt.h>
#include "pgp.h"
#include "envoi.h"
#include "commandes.h"
#include "lire_ecrire.h"
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


