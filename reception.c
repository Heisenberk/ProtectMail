#include <stdio.h>
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
