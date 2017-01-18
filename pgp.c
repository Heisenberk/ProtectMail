#include <stdio.h>
#include <stdlib.h>
#include "pgp.h"
#include "envoi.h"
#include "reception.h"
#include "commandes.h"
#include "lire_ecrire.h"
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
		printf("DECHIFFREMENT\n");
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
