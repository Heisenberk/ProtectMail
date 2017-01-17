#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pgp.h"
#include "envoi.h"
#include "lire_ecrire.h"
#include "types.h"

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


