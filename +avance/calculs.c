#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>
#define TAILLE 1024

int teste_egalite(mpz_t nb,int terme){
	//mpz_t temp;
	mpz_t resultat;
	mpz_init(resultat);
	//mpz_init(temp);
	//mpz_set_d(temp,terme); //le terme dans temp
	mpz_sub_ui(resultat,nb,(unsigned long int)terme);
	if((int)mpz_get_ui(resultat)==0) return 1;
	else return 0;
	
	
}

void affiche_bit(mpz_t bit){
	if(teste_egalite(bit,0)==1) printf("0");
	else if(teste_egalite(bit,-1)==1) printf("1");
	else printf("?");
}

void affiche_binaire(mpz_t nb){
	mpz_t temp;
	mpz_t temp2;
	mpz_t temp3;
	mpz_t deux;
	mpz_t bit;
	mpz_init(bit);
	mpz_init(deux);
	mpz_init(temp2);
	mpz_init(temp3);
	mpz_set_d(deux,2); //on met 2 dans "deux"
	mpz_init_set(temp,nb); //on copie nb dans temp
	
	//gmp_printf("temp=%Zd\n",temp);
	//mpz_cdiv_qr(temp2,bit,nb,deux);
	//gmp_printf("temp=%Zd\n",temp2); //resultat du quotient
	//gmp_printf("r=%Zd\n",bit);
	int compteur=0;
	
	while((teste_egalite(nb,0)!=1)&&(teste_egalite(nb,1)!=1)){
		mpz_mod(bit,nb,deux); //calcul de r
		mpz_sub(temp3,nb,bit);
		mpz_cdiv_q(temp2,temp3,deux);
		gmp_printf("%Zd = %Zd * %Zd + %Zd\n",nb,deux,temp2,bit);
		mpz_init_set(nb,temp2);
		
		
		/*mpz_cdiv_qr(temp2,bit,nb,deux);
		gmp_printf("%Zd = %Zd * %Zd + %Zd\n",nb,deux,temp2,bit);
		mpz_init_set(nb,temp2);*/
	}
	mpz_cdiv_qr(temp2,bit,nb,deux);
	gmp_printf("%Zd = %Zd * %Zd + %Zd\n",nb,deux,temp2,bit);
	printf("\n");
	//mpz_mod(bit,nb,deux);
	//gmp_printf("bit %Zd\n",bit);
}

int main(){
	//TIMER
	gmp_randstate_t state;
	gmp_randinit_default (state);
	gmp_randseed_ui(state,(unsigned)time(NULL));
	///////
	
	mpz_t nb;
	mpz_init(nb);
	mpz_urandomb(nb,state,TAILLE);
	
	gmp_printf("nb=%Zd\n",nb);
	mpz_out_str(NULL,2,nb);
	printf("\n");
	
	return 0;
}
