#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pgp.h"
#include "reception.h"
#include "lire_ecrire.h"
#include "types.h"

int teste_egalite_hashes(unsigned char* hash1, unsigned char* hash2){
	int i;
	for(i=0;i<TAILLE_HASH_MD5;i++){
		if(hash1[i]!=hash2[i]) return 0;
		i++;
	}
	return 1;
}

void calcul_hash_message(char* nomFichier,unsigned char* hash_message){
	
}

void calcul_hash_signature(char* nomFichier,unsigned char* hash_signature){
	
}

int teste_signature(char* nomFichier){
	/*unsigned char* hash_message;
	calcul_hash_message(nomFichier,hash_message);
	unsigned char* signature;
	calcul_hash_signature(nomFichier,signature);
	return teste_egalite_hashes(hash_message,signature);*/
	return 0;//
}

void verifie_authentification(char* nomFichier){
	if(teste_signature(nomFichier)){
		printf("\033[01mAuthentification du fichier \033[31m%s\033[37m vérifié. Visualisation du contenu : \n\n",nomFichier);
		affiche_contenu_fic(nomFichier);
	}
	else demande_visualisation_message(nomFichier);
}

void cree_fichier_dechiffre(char* nomFichier){
	int nb=strlen(nomFichier)-4;
	char new[nb+1];
	int i;
	for(i=0;i<nb;i++){
		new[i]=nomFichier[i];
	}
	new[nb]='\0';
	FILE* f=fopen(new,"w");
	if(f==NULL) exit(1);
	//ECRIRE ICI LE MESSAGE DECHIFFRE DANS LE FILE
	fclose(f);
	printf("\033[01mCréation du fichier déchiffré \033[31m%s\033[0m\n\n",nomFichier);
	
}
