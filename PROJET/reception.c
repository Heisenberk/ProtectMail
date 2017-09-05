//VERIFIE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pgp.h"
#include "reception.h"
#include "gestion_cles.h"
#include "lire_ecrire.h"
#include "math_crypto.h"
#include "types.h"

// Renvoie 1 si hash1 et hash2 sont identiques
int teste_egalite_hashes(unsigned char* hash1, unsigned char* hash2){
	int i;
	for(i=0;i<TAILLE_HASH_MD5;i++){
		if(hash1[i]!=hash2[i]) return 0;
		i++;
	}
	return 1;
}

// A REMPLIR
void calcul_hash_message(char* nomFichier,unsigned char* hash_message){
	
}

// A REMPLIR
void calcul_hash_signature(char* nomFichier,unsigned char* hash_signature){
	
}

// A REMPLIR
int teste_signature(char* nomFichier){
	/*unsigned char* hash_message;
	calcul_hash_message(nomFichier,hash_message);
	unsigned char* signature;
	calcul_hash_signature(nomFichier,signature);
	return teste_egalite_hashes(hash_message,signature);*/
	return 0;//
}

// A UTILISER QUAND TESTE_SIGNATURE REMPLIE
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
	FILE* f2=fopen(nomFichier,"r");
	if(f2==NULL) exit(1);
	CLE my_key;
	my_key.session[16]='\0';
	mpz_t n,d;
	mpz_init(n); mpz_init(d);
	cherche_cle_priv(n,d); //lecture de notre clé privée RSA
	decrypt_rsa_chaine(my_key.session,f2,n,d);  //déchiffrement de la clé de session avec la clé privée RSA
	decrypt_session(my_key,f2,f);
	fclose(f);
	fclose(f2);
	mpz_clear(n);
	mpz_clear(d);
	printf("\033[01mCréation du fichier déchiffré \033[31m%s\033[0m\n\n",new);
	
}

void ecrit_message_dechiffre(char* nomFichier){
	FILE* f2=fopen(nomFichier,"r");
	if(f2==NULL) exit(1);
	CLE my_key;
	my_key.session[16]='\0';
	mpz_t n,d;
	mpz_init(n); mpz_init(d);
	cherche_cle_priv(n,d); //lecture de notre clé privée RSA
	decrypt_rsa_chaine(my_key.session,f2,n,d);  //déchiffrement de la clé de session avec la clé privée RSA
	//decrypt_session(my_key,f2,f);
	decrypt_session_affichage(my_key,f2);
	fclose(f2);
	mpz_clear(n);
	mpz_clear(d);
	//printf("\033[01mCréation du fichier déchiffré \033[31m%s\033[0m\n\n",new);
	
}
