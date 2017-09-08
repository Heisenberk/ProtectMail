//VERIFIE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gmp.h>
#include "reception.h"
#include "gestion_cles.h"
#include "lire_ecrire.h"
#include "math_crypto.h"
#include "types.h"

// Renvoie 1 si hash1 et hash2 sont identiques
int teste_egalite_hashes(char* hash1, char* hash2){
	int i;
	for(i=0;i<TAILLE_HASH_SHA1;i++){
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
	/*if(teste_signature(nomFichier)){
		printf("\033[01mAuthentification du fichier \033[31m%s\033[37m vérifié. Visualisation du contenu : \n\n",nomFichier);
		affiche_contenu_fic(nomFichier);
	}
	else demande_visualisation_message(nomFichier);*/
	
	FILE* origin=fopen(nomFichier,"r");
	int car=compte_nb_car_fichier_signature(origin)+1;
	char* message=malloc(car*sizeof(char));
	remplit_chaine_carac_message_signe(origin,message,car);
	SHA1 hash; //le hash SHA1 est cree
	char final[TAILLE_HASH_SHA1];
	hash=init_sha1(message);
	process_sha1(hash,final);
	done_sha1(hash);
	//printf("\nHASH:%s\n",final);
	
	mpz_t n,e;
	mpz_init(n); mpz_init(e);
	cherche_cle_pub(n,e); //la cle publique est stockee
		
	char* calcul_hash=malloc((TAILLE_HASH_SHA1+1)*sizeof(char));
	calcul_hash[40]='\0';
	decrypt_rsa_chaine(calcul_hash,40,origin,n,e); //calcul le hash sur le message
	
	if(teste_egalite_hashes(final,calcul_hash)){
		printf("\033[01mAuthentification du fichier \033[31m%s\033[37m vérifié. Visualisation du contenu : \n\n",nomFichier);
		printf("\033[34m%s\n\n\033[0m",message);
	}
	else demande_visualisation_message(message);
	
	mpz_clear(n);
	mpz_clear(e);
	free(message);
	free(calcul_hash);
	fclose(origin);
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
	decrypt_rsa_chaine(my_key.session,TAILLE_CLE_SESSION,f2,n,d);  //déchiffrement de la clé de session avec la clé privée RSA
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
	decrypt_rsa_chaine(my_key.session,TAILLE_CLE_SESSION,f2,n,d);  //déchiffrement de la clé de session avec la clé privée RSA
	//decrypt_session(my_key,f2,f);
	decrypt_session_affichage(my_key,f2);
	fclose(f2);
	mpz_clear(n);
	mpz_clear(d);
	//printf("\033[01mCréation du fichier déchiffré \033[31m%s\033[0m\n\n",new);
}

