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
	printf("\033[01m- Pour déchiffrer un fichier :\033[32m ./pgp -w [fichier.pgp]\033[37m Le contenu déchiffré du\n  fichier s'affichera à l'écran\n\033[0m");
	printf("\033[01m- Pour déchiffrer un fichier :\033[32m ./pgp [fichier.pgp]\033[37m Le fichier message déchiffré   sera dans \033[31m[fichier]\n\033[0m");
	printf("\033[01m- Pour signer un message :\033[32m ./pgp -s [fichier]\n\033[0m");
	
	printf("\n\033[0m");
}

void affiche_action_pgp(char* nom){
	printf("\033[01mLe fichier à envoyer est sous le nom : \033[31m%s\n\n\033[0m",nom);
}

void ecrit_bordure_sup_rsa_pub(FILE* f){
	fprintf(f,"-----BEGIN PGP PUBLIC KEY BLOCK-----\n");
}

void ecrit_bordure_sup_rsa_priv(FILE* f){
	fprintf(f,"-----BEGIN PGP PRIVATE KEY BLOCK-----\n");
}

void ecrit_bordure_sup_m_sig(FILE* f){
	fprintf(f,"-----BEGIN PGP SIGNED MESSAGE-----\n");
}

void ecrit_bordure_sup_id(FILE* f,char* s1,char* s2,char* s3){
	fprintf(f,"-----IDENTITY-----\n");
	fprintf(f,"%s %s %s\n",s1,s2,s3);
}

void ecrit_bordure_inf_m_sig(FILE* f){
	fprintf(f,"\n-----BEGIN PGP SIGNATURE-----\n");
}

void ecrit_bordure_inf_rsa_pub(FILE* f){
	fprintf(f,"\n-----END PGP PUBLIC KEY BLOCK-----\n");
}

void ecrit_bordure_inf_rsa_priv(FILE* f){
	fprintf(f,"\n-----END PGP PRIVATE KEY BLOCK-----\n");
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

void affiche_dechiffrement(char* nomFichier){
	printf("AFFICHAGE FICHIER DECHIFFRE\n\n");
}

void affiche_fichier_dechiffre(char* nomFichier){
	printf("\033[01mLe contenu déchiffré du fichier \033[31m%s\033[37m est : \n\n\033[0m",nomFichier);
	affiche_dechiffrement(nomFichier);
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

void cree_pass_phrase(){
	printf("\033[01mVous devez entrer une Pass Phrase pour protéger votre clé secrète RSA.");
	printf("\033[01mEntrez la Pass Phrase: \033[0m\033[30m");
	char buffer1[256],buffer2[256];
	scanf("%s",buffer1);
	printf("\033[0m");
	printf("\033[01mEntrez de nouveau la Pass Phrase: \033[0m\033[30m");
	scanf("%s",buffer2);
	printf("\033[0m");
	/*if(!strcmp(buffer1,buffer2)){
		int taille=strlen(buffer1);
		unsigned char* mdp=(unsigned char*)buffer1;
		md5(mdp,taille,hash);
	}*/
	//else quitte_pass_phrase_incoherente();
}

void ecrit_cle_privee(){
	FILE* f=fopen("secring.pgp","w");
	ecrit_bordure_sup_rsa_priv(f);
	// genere et ecrit la cle ici
	ecrit_bordure_inf_rsa_priv(f);
	fclose(f);
	printf("\033[01m\033[31mGénération de la clé privée terminée\n\n\033[0m");
}

//ATTENTION PENSER AU FAIT QUE LE FICHIER DE CLEFS EST DEJA REMPLIE
//DE CONTACTS QUON NE VEUT PAS PERDRE!!!!
//A REFAIRE
void ecrit_cle_publique(char* s1,char* s2,char* s3){
	FILE* f=fopen("pubring.pgp","w");
	ecrit_bordure_sup_id(f,s1,s2,s3);
	ecrit_bordure_sup_rsa_pub(f);
	//genere et ecrit la cle ici
	ecrit_bordure_inf_rsa_pub(f);
	fclose(f);
	printf("\033[01m\033[31m\nGénération de la clé publique terminée\n\n\033[0m");
}
