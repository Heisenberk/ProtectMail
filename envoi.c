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

void md5(unsigned char* in,int taille,unsigned char* out){
	hash_state md;
	//unsigned char out[16];
	md5_init(&md);
	md5_process(&md,in,taille);
	md5_done(&md,out);
}

void demande_pass_phrase(unsigned char* hash){
	printf("\033[01mEntrez la Pass Phrase: \033[0m\033[30m");
	char buffer1[256],buffer2[256];
	scanf("%s",buffer1);
	printf("\033[0m");
	printf("\033[01mEntrez de nouveau la Pass Phrase: \033[0m\033[30m");
	scanf("%s",buffer2);
	printf("\033[0m");
	if(!strcmp(buffer1,buffer2)){
		int taille=strlen(buffer1);
		unsigned char* mdp=(unsigned char*)buffer1;
		md5(mdp,taille,hash);
	}
	else quitte_pass_phrase_incoherente();
}

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


