#include <stdio.h>
#include <stdlib.h>
#include "pgp.h"
#include "envoi.h"
#include "reception.h"
#include "commandes.h"
#include "lire_ecrire.h"
#include "gestion_cles.h"
#include "types.h"

/*void affiche(unsigned char* h){
	int i;
	for(i=0;i<16;i++){
		printf("%x",h[i]);
	}
	printf("\n");
}

void affiche2 (char* s){
	int i=0;
	//char c=s[i];
	while(s[i]!='\0'){
		printf("%c",s[i]);
		i++;
	}
	printf("\n");
}*/

int main(int argc,char** argv){
	int mode=teste_commande_general(argc,argv);
	if(mode==MODE_SIGN_NN_CHIFFRE){
		ecrit_message_non_chiffre(argc,argv);
	}
	else if(mode==MODE_DECHIFFREMENT){
		cree_fichier_dechiffre(argv[1]);
	}
	else if(mode==MODE_AFFICHAGE_DECHIFFRE){
		affiche_fichier_dechiffre(argv[2]);
	}
	else if(mode==MODE_GENERATION_CLES){
		//genere_cle_privee();
		//genere_cle_publique();
		genere_cles();
	}
	else if(mode==MODE_TRANSFERT_CLES){
		transfert_fic1_fic2(argv[2],argv[3]);
	}
	return 0;
	
	/*verifie_authentification(argv[1]);
	return 0;*/
	
	/*unsigned char hash[16];
	demande_pass_phrase(hash);
	affiche(hash);
	char* hash1=PASSPHRASE;
	unsigned char* final=(unsigned char*)hash1;
	affiche2(hash1);
	//if(teste_egalite_hashes(hash,final)) printf("EGALITE\n");
	//else printf("INEGALITE\n");
	return 0;*/
}
