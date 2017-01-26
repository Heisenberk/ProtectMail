#include <stdio.h>
#include <tomcrypt.h>
#include <gmp.h>
#include "pgp.h"
#include "commandes.h"
#include "lire_ecrire.h"

// Fonction GMP pour initialiser les variables
void initialise_memoire(mpz_t p,mpz_t q,mpz_t n,mpz_t z,mpz_t e,mpz_t d){
	mpz_init(p);
	mpz_init(q);
	mpz_init(n);
	mpz_init(z);
	mpz_init(e);
	mpz_init(d);
}

// Fonction GMP pour libérer la mémoire
void libere_memoire(mpz_t p,mpz_t q,mpz_t n,mpz_t z,mpz_t e,mpz_t d,gmp_randstate_t state){
	mpz_clear(p);
	mpz_clear(q);
	mpz_clear(n);
	mpz_clear(z);
	mpz_clear(e);
	mpz_clear(d);
	gmp_randclear(state);
}

// Met dans p un nombre premier aléatoire
// p est le futur nombre premier, state correspond à l'aléatoire et choix correspond à la taille du nombre
void determine_premier(mpz_t p,gmp_randstate_t state,int choix){
	mpz_urandomb(p,state,choix); //met dans p un nombre aleatoire
	int test=mpz_probab_prime_p(p,10); //renvoie 0 si c'est pas premier, 1 si c'est surement premier et 2 si c'est premier (sur)
	while(((test!=1)&&(test!=2))){ //continue de choisir un nombre tant qu'il n'est pas premier
		mpz_urandomb(p,state,choix);
		test=mpz_probab_prime_p(p,10); //10 est une valeur correcte pour faire le test (voir manuel gmp)
	}
}

// Met dans n le produit de p et q
void determine_n(mpz_t p,mpz_t q,mpz_t n){
	mpz_addmul(n,p,q);
}

// Met dans n le produit de p-1 et q-1
void determine_z(mpz_t p,mpz_t q,mpz_t z){
	mpz_t pp,ppp,qq,qqq;
	mpz_init(ppp);
	mpz_init(qqq);
	mpz_init_set(pp,p); //copie p dans pp
	mpz_init_set(qq,q); //copie q dans qq
	mpz_sub_ui(ppp,pp,1); //ppp=pp-1
	mpz_sub_ui(qqq,qq,1); //qqq=qq-1
	mpz_addmul(z,ppp,qqq); //z=ppp*qqq
	mpz_clear(pp);
	mpz_clear(ppp);
	mpz_clear(qq);
	mpz_clear(qqq);
}

// Met dans e la valeur tel que pgcd(e,z)=1
void determine_e(mpz_t z,gmp_randstate_t state,mpz_t e){
	mpz_t pgcd; //variable qui contiendra le pgcd
	mpz_init(pgcd);
	int test;
	do{
		mpz_urandomb(e,state,TAILLE); //choisit un nb aleatoire
		mpz_gcd(pgcd,e,z); //CALCUL PGCD(e,z);
		test=mpz_cmp_d(pgcd,1); //PGCD=1?
	}while(test!=0); //tant que pgcd n'est pas égal a 1 on continue
	mpz_clear(pgcd);
}

// Met dans d la valeur de l'inverse de e modulo z
void determine_d(mpz_t p,mpz_t q,mpz_t n,mpz_t z,mpz_t e,mpz_t d,gmp_randstate_t state){
	int calc=mpz_invert(d,e,z); //met dans d l'inverse multiplicatif de e mod z
	if(calc==0){ //si c'est pas possible on quitte
		libere_memoire(p,q,n,z,e,d,state);
		printf("CLES INDETERMINEES\n");
		exit(1);
	}
}

// A SUPPRIMER
void affiche_cles(mpz_t e,mpz_t d,mpz_t n){
	gmp_printf("CLE PUBLIQUE (%Zd,%Zd)\n",e,n);
	gmp_printf("CLE PRIVEE   (%Zd,%Zd)\n",d,n);
}

// Chiffre "nomFichier" grâce à la clé publique (e,n)
void encrypt(char* nomFichier,mpz_t n,mpz_t e){
	FILE* f1=fopen(nomFichier,"r");
	if(f1==NULL){
		printf("FICHIER INEXISTANT\n");
		exit(1);
	}
	FILE* f2=fopen("message_chiffre","w");
	if(f1==NULL){
		printf("FICHIER IMPOSSIBLE A CREER\n");
		exit(1);
	}
	char c;double ascii;mpz_t m,u;int test;
	int compteur=0; //
	mpz_init(m);mpz_init(u);
	do{
		c=fgetc(f1);
		ascii=c;
		if(c!=EOF){
			mpz_set_d(m,ascii); //met dans m --> ascii
			mpz_powm(u,m,e,n); //u=m^e mod n
			test=gmp_fprintf(f2,"%Zd ",u);
			if(test==-1) exit(1); //
			compteur++;//
			if((compteur%5)==0)fprintf(f2,"\n"); //
		}
	}while(c!=EOF);
	fclose(f1);
	fclose(f2);
	mpz_clear(m);
	mpz_clear(u);
	printf("\nMESSAGE CHIFFRE\n");
}

// Déchiffre "nomFichier" grâce à la clé privée (d,n)
void decrypt(char* nomFichier,mpz_t n,mpz_t d){
	FILE* f1=fopen(nomFichier,"r");
	if(f1==NULL){
		printf("FICHIER INEXISTANT\n");
		exit(1);
	}
	FILE* f2=fopen("message_dechiffre","w");
	if(f1==NULL){
		printf("FICHIER IMPOSSIBLE A CREER\n");
		exit(1);
	}
	char c;mpz_t m,u;int test;unsigned long int conv;int car;
	mpz_init(m);mpz_init(u);
	do{
		c=fgetc(f1);
		if((c!=EOF)&&(isdigit(c))){
			ungetc(c,f1);
			test=gmp_fscanf(f1,"%Zd",m);
			if(test==-1) exit(1); //
			mpz_powm(u,m,d,n); //u=m^d mod n
			conv=mpz_get_ui(u); // car prend la valeur de u
			car=conv;
			fprintf(f2,"%c",car);
			while(c!=' ') c=fgetc(f1);
		}
	}while(c!=EOF);
	fclose(f1);
	fclose(f2);
	mpz_clear(m);
	mpz_clear(u);
	printf("\nMESSAGE DECHIFFRE\n");
}
////

// Met dans out le contenu de md5 de in
void md5(unsigned char* in,int taille,unsigned char* out){
	hash_state md;
	//unsigned char out[16];
	md5_init(&md);
	md5_process(&md,in,taille);
	md5_done(&md,out);
}

//CHANGER LES NOMS
void genere_cle_privee(mpz_t n,mpz_t e){
	ecrit_cle_privee(n,e);
}

//CHANGER LES NOMS
void genere_cle_publique(mpz_t n,mpz_t e){
	char prenom[64];
	char nom[64];
	char adresse[64];
	//A DEPLACER
	printf("\033[01mVotre clé publique nécessite un prénom, un nom suivi de votre adresse mail fermée par les symboles < et > Par exemple : John Smith <1234.567@mail.com>\n");
	printf("Entrez votre prénom : \033[33m");
	scanf("%s",prenom);
	printf("\033[37mEntrez votre nom : \033[33m");
	scanf("%s",nom);
	printf("\033[37mEntrez votre mail entre crochets : \033[33m");
	scanf("%s",adresse);
	printf("\033[0m");
	//
	ecrit_cle_publique(prenom,nom,adresse,n,e);
}

// Génère les clés aléatoirement
//changer ici les parametres pr pouvoir ecrire les clefs 
void genere_cles(){
	//TIMER : state permet de choisir SEED pour améliorer le pseudo aléatoire
	gmp_randstate_t state;
	gmp_randinit_default (state);
	gmp_randseed_ui(state,(unsigned)time(NULL));
	///////
	mpz_t p,q,n,z,e,d;
	int choix=demande_taille_cles();
	initialise_memoire(p,q,n,z,e,d);
	determine_premier(p,state,choix);
	determine_premier(q,state,choix);
	determine_n(p,q,n);
	determine_z(p,q,z);
	determine_e(z,state,e);
	determine_d(p,q,n,z,e,d,state);
	/*gmp_printf("p=%Zd\n",p);
	gmp_printf("q=%Zd\n",q);
	gmp_printf("n=%Zd\n",n);
	gmp_printf("z=%Zd\n",z);
	gmp_printf("e=%Zd\n",e);
	gmp_printf("d=%Zd\n",d);
	affiche_cles(e,d,n);*/
	//A MODIFIER CAR SI ON TAPE 2 PASSES PHRASES DIFFERENTES 
	//IL Y A QUAND MEME ECRIT PUBLIC (PAS PRIVE)
	genere_cle_publique(n,e);
	genere_cle_privee(n,d);
	printf("\033[01m\033[31m\nGénération des clés publique et privée terminée\n\n\033[0m");
	libere_memoire(p,q,n,z,e,d,state);
		
}

// Met dans buffer1 la Pass Phrase choisie par l'utilisateur
void cree_pass_phrase(char* buffer1){
	printf("\033[01mVous devez entrer une Pass Phrase pour protéger votre clé secrète RSA.\n");
	printf("\033[01mEntrez la Pass Phrase: \033[0m\033[30m");
	/*char buffer1[256],*/ char buffer2[256];
	scanf("%s",buffer1);
	printf("\033[0m");
	printf("\033[01mEntrez de nouveau la Pass Phrase: \033[0m\033[30m");
	scanf("%s",buffer2);
	printf("\033[0m");
	if(strcmp(buffer1,buffer2)){ //si ce n'est pas identique
		quitte_pass_phrase_incoherente();
	}
}

// A MODIFIER OU A SUPPRIMER
void demande_pass_phrase(unsigned char* hash){
	printf("\033[01mEntrez la Pass Phrase: \033[0m\033[30m");
	char buffer1[256],buffer2[256];
	scanf("%s",buffer1);
	printf("\033[0m");
	printf("\033[01mEntrez de nouveau la Pass Phrase: \033[0m\033[30m");
	scanf("%s",buffer2);
	printf("\033[0m");
	if(!strcmp(buffer1,buffer2)){
		int taille=strlen(buffer1);
		unsigned char* mdp=(unsigned char*)buffer1;
		md5(mdp,taille,hash);
	}
	else quitte_pass_phrase_incoherente();
}
