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

int compteur_carac(FILE* f){
	int compteur=-1;
	char c='a';
	while(c!='\n'){
		compteur++;
		c=fgetc(f);
	}
	return compteur;
}

void remplit_tableau(FILE* f,char* tab,int max){
	int i;
	char c;
	for(i=0;i<max-1;i++){
		c=fgetc(f);
		tab[i]=c;
	}
	fgetc(f);
	tab[i++]='\0';
}

void affiche_tab(char* tab,int max){
	int i;
	for(i=0;i<max-1;i++){
		printf("%c",tab[i]);
	}
	//if(tab[i++]=='\') printf("f");
	printf("\n\n");
}

void euclide1(mpz_t a, mpz_t b,mpz_t q){
	mpz_t n,p;
	mpz_init(n);
	mpz_init(p);
	mpz_set_d(n,0);
	mpz_set(p,b);
	//int compteur=0;
	while(mpz_cmp(p,a)<0){ //while(p<=a);
		mpz_mul_si(p,p,2); //p=(p<<1)
		mpz_add_ui(n,n,1); //n++
		//compteur++;
	}
	
	/*char resultat[1024];
	char* pt=mpz_get_str (resultat,2,p);
	printf("COMPTEUR=%d ETAPE 1: \n%s\n",compteur,resultat);*/
	
	mpz_sub_ui(n,n,1); //n--
	mpz_fdiv_q_2exp (p,p,1); //p=(p>>1);
	
	/*char resultat1[1024];
	char* pt2=mpz_get_str (resultat1,2,n);
	printf("ETAPE 2: \n%s\n",resultat1);*/
	
	int n_bis=mpz_get_ui(n);
	
	mpz_init(q);
	mpz_set_d(q,1);
	mpz_mul_2exp(q,q,n_bis); //q=(1<<n);
	
	/*char resultat2[1024];
	char* pt3=mpz_get_str (resultat2,2,q);
	printf("ETAPE 4: \n%s\n",resultat2);*/
	
	mpz_t aux;
	mpz_init(aux);
	mpz_set(aux,p); //aux=p;
	
	mpz_t zero;
	mpz_t som;
	mpz_t un_shifte;
	mpz_init(zero);
	int n_bis_bis;
	
	mpz_set_d(zero,0);
	int compteur1=0;
	int compteur2=0;
	while(mpz_cmp(n,zero)>0){ //while(n>0)
		compteur1++;
		mpz_fdiv_q_2exp (p,p,1); //p=(p>>1);
		mpz_sub_ui(n,n,1); //n--;
		
		n_bis_bis=mpz_get_ui(n);
		mpz_init(som);
		mpz_init(un_shifte);
		mpz_add(som,aux,p);
		if(mpz_cmp(som,a)<0){ //if((aux+p)<=a)
			compteur2++;
			mpz_mul_2exp(un_shifte,q,n_bis_bis); //un_shifte=(1<<n);
			mpz_add(q,q,un_shifte); //q+=(1<<n)
			mpz_add(aux,aux,p); //aux+=p
		}
		mpz_clear(som);
		mpz_clear(un_shifte);
	}
	
	/*char resultat3[1024];
	char* pt4=mpz_get_str (resultat3,2,n);
	printf("ETAPE 6: \n%s\n",resultat3);
	
	char resultat4[1024];
	char* pt5=mpz_get_str (resultat4,2,p);
	printf("ETAPE 7: \n%s\n",resultat4);*/
	
	char resultat5[1024];
	char* pt6=mpz_get_str (resultat5,10,q);
	printf("RESULTAT DANS MA FONCTION \n%s\n",resultat5);
	//DIFFERENT JAI LIMPRESSION
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
	/*int a=146464;
	int b=5697;
	int c=euclide(a,b);
	printf("%d/%d=%d\n",a,b,c);*/
	mpz_t a,b,c;
	mpz_init(a);
	mpz_init(b);
	mpz_init(c);
	int compteur1,compteur2;
	FILE* f=fopen("test","r");
	compteur1=compteur_carac(f);
	//printf("COMPTEUR1: %d\n",compteur1);
	fseek(f,0,SEEK_SET);
	int taille1=compteur1+1;
	char nb1[taille1];
	remplit_tableau(f,nb1,taille1);
	//affiche_tab(nb1,taille1);
	mpz_set_str(a,nb1,2);
	gmp_printf("%Zd\n", a);
	printf("/\n");
	
	compteur2=compteur_carac(f);
	//printf("COMPTEUR2: %d\n",compteur2);
	fseek(f,-compteur2,SEEK_CUR);
	int taille2=compteur2+1;
	char nb2[taille2];
	remplit_tableau(f,nb2,taille2);
	//affiche_tab(nb2,taille2);
	mpz_set_str(b,nb2,2);
	gmp_printf("%Zd\n", b);
	printf("=\n");
	
	mpz_cdiv_q(c,a,b);
	
	printf("RESULTAT REEL :\n");
	gmp_printf("%Zd\n",c);
	//char resultat[1024];
	//char* p=mpz_get_str (resultat,2,c);
	//printf("%s\n\n\n",resultat);
	
	printf("TEST EUCLIDE:\n");
	mpz_t qq;
	mpz_init(qq);
	euclide1(a,b,qq);

	mpz_clear(a);
	mpz_clear(b);
	mpz_clear(c);
	fclose(f);
	
	//printf("%d\n", mpz_cmp(a,a)); //=0 a>b
	//euclide(150,8);
	/*mpz_t qqq;
	mpz_init(qqq);
	mpz_set_d(qqq,100);
	mpz_fdiv_q_2exp (qqq,qqq,1);
	gmp_printf("%Zd\n", qqq);*/
	
	
	return 0;
}
