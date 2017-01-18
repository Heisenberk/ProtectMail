#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pgp.h"
#include "commandes.h"
#include "lire_ecrire.h"
#include "types.h"

void quitte_commande_introuvable(){
	printf("\033[01mCOMMANDE INTROUVABLE\n\n\033[0m");
	exit(1); //revoir le chiffre
}

void quitte_fichier_inexistant(){
	printf("\033[01mFICHIER INEXISTANT\n\n\033[0m");
	exit(1); //
}

void quitte_pass_phrase_incoherente(){
	printf("\033[01m\nLES DEUX PASS PHRASES SONT DIFFERENTES\n\n\033[0m");
	exit(1);//
}

int teste_mots_identiques(char* s1,char* s2){
	if(strcmp(s1,s2)==0) return 1;
	return 0;
}

int teste_commande_une_option(int num,char** chaineCarac){
	if(teste_mots_identiques(chaineCarac[1],"-h")){
		affiche_commandes();
		exit(1); //
	}
	return 1;
}

int teste_commande_deux_options(int num,char** chaineCarac){
	if(teste_mots_identiques(chaineCarac[1],"-s")){
		FILE* f=fopen(chaineCarac[2],"r");
		if(f==NULL) quitte_fichier_inexistant();
		fclose(f);
		return MODE_SIGN_NN_CHIFFRE;
	}
	return 1;
}

int teste_commande_general(int num,char** chaineCarac){
	affiche_general();
	if(num==1){}
	else if(num==2) return teste_commande_une_option(num,chaineCarac);
	else if(num==3) return teste_commande_deux_options(num,chaineCarac);
	else quitte_commande_introuvable();
	return 0;
}
