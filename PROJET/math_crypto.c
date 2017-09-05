//VERIFIE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tomcrypt.h>
#include <gmp.h>
#include "pgp.h"
#include "envoi.h"
#include "commandes.h"
#include "lire_ecrire.h"
#include "gestion_cles.h"
#include "types.h"

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

// a modifier pour qur ce soit pas aleatoire
//calcul euclide etendu
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

int random_int(gmp_randstate_t state){
	mpz_t n;
	mpz_init(n);
	mpz_urandomb(n,state,7);
	double i;
	i=mpz_get_d(n);
	if((i<=31)||(i>=127)) i=random_int(state);
	mpz_clear(n);
	int ii=i;
	return ii;
}

int xor(int i,int j){
	return i ^ j;
}
