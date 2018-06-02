#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>
#define TAILLE 500 //taille pour 1024 bits

void libere_memoire(mpz_t p,mpz_t q,mpz_t n,mpz_t z,mpz_t e,mpz_t d,gmp_randstate_t state){
	mpz_clear(p);
	mpz_clear(q);
	mpz_clear(n);
	mpz_clear(z);
	mpz_clear(e);
	mpz_clear(d);
	gmp_randclear(state);
}

void initialise_memoire(mpz_t p,mpz_t q,mpz_t n,mpz_t z,mpz_t e,mpz_t d){
	mpz_init(p);
	mpz_init(q);
	mpz_init(n);
	mpz_init(z);
	mpz_init(e);
	mpz_init(d);
}

void determine_premier(mpz_t p,gmp_randstate_t state){
	mpz_urandomb(p,state,TAILLE);
	int test=mpz_probab_prime_p(p,10);
	while(((test!=1)&&(test!=2))){
		mpz_urandomb(p,state,TAILLE);
		test=mpz_probab_prime_p(p,10);
	}
}

void determine_n(mpz_t p,mpz_t q,mpz_t n){
	mpz_addmul(n,p,q);
}

void determine_z(mpz_t p,mpz_t q,mpz_t z){
	mpz_t pp,ppp,qq,qqq;
	mpz_init(ppp);
	mpz_init(qqq);
	mpz_init_set(pp,p);
	mpz_init_set(qq,q);
	mpz_sub_ui(ppp,pp,1);
	mpz_sub_ui(qqq,qq,1);
	mpz_addmul(z,ppp,qqq);
	mpz_clear(pp);
	mpz_clear(ppp);
	mpz_clear(qq);
	mpz_clear(qqq);
}

void determine_e(mpz_t z,gmp_randstate_t state,mpz_t e){
	mpz_t pgcd;
	mpz_init(pgcd);
	int test;
	do{
		mpz_urandomb(e,state,TAILLE);
		mpz_gcd(pgcd,e,z); //CALCUL PGCD
		test=mpz_cmp_d(pgcd,1); //PGCD=1?
	}while(test!=0);
	mpz_clear(pgcd);
}


//CALCUL DE LINVERSE DE E MOD Z
void determine_d(mpz_t p,mpz_t q,mpz_t n,mpz_t z,mpz_t e,mpz_t d,gmp_randstate_t state){
	int calc=mpz_invert(d,e,z);
	if(calc==0){
		libere_memoire(p,q,n,z,e,d,state);
		printf("CLES INDETERMINEES\n");
		exit(1);
	}
}

void affiche_cles(mpz_t e,mpz_t d,mpz_t n){
	gmp_printf("CLE PUBLIQUE (%Zd,%Zd)\n",e,n);
	gmp_printf("CLE PRIVEE   (%Zd,%Zd)\n",d,n);
}

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
			mpz_set_d(m,ascii);
			mpz_powm(u,m,e,n);
			test=gmp_fprintf(f2,"%Zd ",u);
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
	char c;double ascii;mpz_t m,u;int test;unsigned long int conv;int car;
	mpz_init(m);mpz_init(u);
	do{
		c=fgetc(f1);
		if((c!=EOF)&&(isdigit(c))){
			ungetc(c,f1);
			test=gmp_fscanf(f1,"%Zd",m);
			mpz_powm(u,m,d,n);
			conv=mpz_get_ui(u);
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

int main(int argc,char**argv){
	//TIMER
	gmp_randstate_t state;
	gmp_randinit_default (state);
	gmp_randseed_ui(state,(unsigned)time(NULL));
	///////
	
	mpz_t p,q,n,z,e,d;
	/*mpz_init(p);
	mpz_init(q);
	mpz_init(n);
	mpz_init(z);
	mpz_init(e);
	mpz_init(d);*/
	initialise_memoire(p,q,n,z,e,d);
	determine_premier(p,state);
	determine_premier(q,state);
	determine_n(p,q,n);
	determine_z(p,q,z);
	determine_e(z,state,e);
	determine_d(p,q,n,z,e,d,state);
	
	printf("\n");
	gmp_printf("p=%Zd\n",p);
	/*char resultat5[1024];
	char* pt6=mpz_get_str (resultat5,2,p);
	printf("RESULTAT DANS MA FONCTION \n%s\n",resultat5);*/
	/*gmp_printf("q=%Zd\n",q);
	gmp_printf("n=%Zd\n",n);
	gmp_printf("z=%Zd\n",z);
	gmp_printf("e=%Zd\n",e);
	gmp_printf("d=%Zd\n",d);*/
	
	affiche_cles(e,d,n);
	encrypt("test",n,e);
	decrypt("message_chiffre",n,d);
	
	libere_memoire(p,q,n,z,e,d,state);
}
