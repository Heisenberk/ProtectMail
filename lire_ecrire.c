#include <time.h>
#include <stdio.h>
#include <string.h>
#include "pgp.h"
#include "lire_ecrire.h"
#include "commandes.h"
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
	printf("\033[01m-Pour déchiffrer un fichier :\033[32m ./pgp [fichier.pgp]\n\033[0m");
	printf("\033[01m-Pour signer un message :\033[32m ./pgp -s [fichier]\n\033[0m");
	
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

void affiche_contenu_fic(char* nomFichier){
	FILE* f=fopen(nomFichier,"r");
	char c;
	do{
		c=fgetc(f);
		if(c!=EOF) printf("\033[34m%c",c);
	}while(c!=EOF);
	printf("\n\033[0m");
	fclose(f);
}

int teste_reponse(char* s){
	int o1=strcmp(s,"oui"); int n1=strcmp(s,"non");
	int o2=strcmp(s,"Oui"); int n2=strcmp(s,"Oui");
	int o3=strcmp(s,"OUI"); int n3=strcmp(s,"OUI");
	int o4=strcmp(s,"OuI"); int n4=strcmp(s,"OuI");
	int o5=strcmp(s,"oUi"); int n5=strcmp(s,"oUi");
	int o6=strcmp(s,"oUI"); int n6=strcmp(s,"oUI");
	int o7=strcmp(s,"O"); int n7=strcmp(s,"N");
	int o8=strcmp(s,"o"); int n8=strcmp(s,"n");
	if((!o1)||(!o2)||(!o3)||(!o4)||(!o5)||(!o6)||(!o7)||(!o8)){
		return 1;
	}
	if((!n1)||(!n2)||(!n3)||(!n4)||(!n5)||(!n6)||(!n7)||(!n8)){
		return 2;
	}
	else return -1;
}

void demande_visualisation_message(char* nomFichier){
	printf("\033[01mFichier corrompu. Voulez-vous quand même l'afficher?(o/N)");
	char buffer[256];
	scanf("%s",buffer);
	if(teste_reponse(buffer)==1) affiche_contenu_fic(nomFichier);
	else quitte_pas_probleme();
	
}
