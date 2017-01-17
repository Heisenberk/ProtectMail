#include <time.h>
#include <stdio.h>
#include "pgp.h"
#include "lire_ecrire.h"
#include "types.h"

void affiche_date_heure(){
	time_t now = time (NULL);
	struct tm tm_now = *localtime (&now);
	char s_now[sizeof "JJ/MM/AAAA HH:MM:SS"];
	strftime (s_now, sizeof s_now, "%d/%m/%Y %H:%M:%S", &tm_now);
	printf ("%s\n", s_now);
}

void affiche_general(){
	printf("\033[01m\nSimplified Privacy Guard - Hybrid Cryptography.\n");
	printf("Université Versailles Saint Quentin en Yvelines - 2017 \nClaire Baskevitch - Clément Caumes\n");
	printf("Date courante : ");
	affiche_date_heure();
	printf("\n\033[0m");
}

void affiche_commandes(){
	printf("\033[01m");
	printf("-Pour signer un message :\033[32m ./pgp -s [fichier]\n");
	
	printf("\n\033[0m");
}

void affiche_action_pgp(char* nom){
	printf("\033[01mLe fichier à envoyer est sous le nom : \033[31m%s\n\n\033[0m",nom);
}

void ecrit_bordure_sup_m_sig(FILE* f){
	fprintf(f,"-----BEGIN PGP SIGNED MESSAGE-----\n");
}

void ecrit_bordure_inf_m_sig(FILE* f){
	fprintf(f,"\n-----BEGIN PGP SIGNATURE-----\n");
}

void recopie_message(FILE* origin,FILE* new){
	char c='a';
	while(c!=EOF){
		c=fgetc(origin);
		if(c!=EOF){
			fprintf(new,"%c",c);
		}

	}
}
