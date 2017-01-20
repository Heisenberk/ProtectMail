#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>
#define TAILLE 25

libere_memoire(mpz_t p,mpz_t q,mpz_t n,mpz_t z,mpz_t e,mpz_t d,gmp_randstate_t state){
	mpz_clear(p);
	mpz_clear(q);
	mpz_clear(n);
	mpz_clear(z);
	mpz_clear(e);
	mpz_clear(d);
	gmp_randclear(state);
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

/*int genere_d(int e,int z){
	int d=0;
	while((e*d)%z!=1){
		d=d+1;
	}
	return d;
}*/

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

int main(int argc,char**argv){
	//TIMER
	gmp_randstate_t state;
	gmp_randinit_default (state);
	gmp_randseed_ui(state,(unsigned)time(NULL));
	///////
	
	mpz_t p,q,n,z,e,d;
	mpz_init(p);
	mpz_init(q);
	mpz_init(n);
	mpz_init(z);
	mpz_init(e);
	mpz_init(d);
	determine_premier(p,state);
	determine_premier(q,state);
	determine_n(p,q,n);
	determine_z(p,q,z);
	determine_e(z,state,e);
	determine_d(p,q,n,z,e,d,state);
	
	gmp_printf("p=%Zd\n",p);
	gmp_printf("q=%Zd\n",q);
	gmp_printf("n=%Zd\n",n);
	gmp_printf("z=%Zd\n",z);
	gmp_printf("e=%Zd\n",e);
	gmp_printf("d=%Zd\n",d);
	
	affiche_cles(e,d,n);
	
	libere_memoire(p,q,n,z,e,d,state);
}
