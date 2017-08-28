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

void affiche_bin(int n){
	int i;
	unsigned bit=0;
	unsigned mask=1;
	for(i=0;i<64;i++){
		bit=(n&mask)>>i;
		printf("%d",bit);
		mask<<=1;
	}
}

int euclide(int a,int b){
	int n=0;
	int p=b;
	
	if(a<b){
		return 0;
	}
	while(p<=a){
		p=(p<<1);
		n++;
	}
	printf("etape1: p=%d OK\n",p);
	p=(p>>1);
	printf("etape2: p=%d \n",p);
	n--;
	printf("etape3: n=%d \n",n);
	int q=(1<<n);
	printf("etape4: q=%d \n",q);
	int aux=p;
	printf("etape5: aux=%d \n",aux);
	while(n>0){
		p=(p>>1);
		n--;
		if((aux+p)<=a){
			q+=(1<<n);
			aux+=p;
			printf("A\n");
		}
	}
	printf("etape6: n=%d \n",n);
	printf("etape7: p=%d \n",p);
	printf("etape8: aux=%d \n",aux);
	printf("etape 9: Q=%d \n",q);
	return q;
}

int main(){
	/*//TIMER
	gmp_randstate_t state;
	gmp_randinit_default (state);
	gmp_randseed_ui(state,(unsigned)time(NULL));
	///////
	
	mpz_t nb;
	mpz_init(nb);
	mpz_urandomb(nb,state,TAILLE);
	
	gmp_printf("nb=%Zd\n",nb);
	mpz_out_str(NULL,2,nb);
	printf("\n");*/
	int a=146464;
	int b=5697;
	int c=euclide(a,b);
	printf("%d/%d=%d\n",a,b,c);
	
	return 0;
}
