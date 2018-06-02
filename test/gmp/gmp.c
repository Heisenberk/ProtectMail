
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <inttypes.h>
#include <time.h>
#include "gmp.h"

/* Renvoie le nombre de bits (sans les 0 à gauche) d'un nombre de type uint64 */
int nb_bits_uint64(uint64_t a)
{
	int nb=0;
	while(a){ nb++; a = a>>1; }
	return nb;
	
}

/* Renvoie le nombre de bits d'une variable de type UINT_X */
int nb_bits_uint_x(UINT_X a)
{
	int nb=0,i;
	for(i=0;i<a.taille;i++) nb += nb_bits_uint64(a.tab[i]);
	return nb;
}

/* Renvoie le bit à un certain rang
 * Le premier bit est au rang 1 */
char access_bit_n(uint64_t a,int n)
{
	if(n<=0 || n>64) return -1;
	a>>=(n-1);
	return a&1;
}

/* Alloue et initialise une variable dont la taille (nombre de bits) est donnée en argument */
UINT_X malloc_uint_x(int taille){
	UINT_X new;
	new.taille = taille/(8*sizeof(uint64_t));
	new.tab = calloc(new.taille, sizeof(uint64_t));
	if(new.tab==NULL){
		fprintf(stderr, "IMPOSSIBLE DE RESERVER DE LA MEMOIRE POUR UINT_X\n");
		exit(EXIT_FAILURE);
	}
	return new;
}

/* Initialise une variable UINT_X à 0 */
void init_zero(UINT_X *n){
	memset(n->tab, 0, n->taille*8);
}



/* Libère la variable de type uint_x */
void free_uint_x(UINT_X n){
	free(n.tab);
}

/* Fait la copie d'un UINT_X
 * il faut que copie ait été initialisé et avec la bonne taille
 * Return value : on success return 1, on failure return -1 */
int copier(UINT_X *copie, UINT_X original)
{
	int i;
	if(copie->tab == NULL || copie->taille<original.taille)
		return fprintf(stderr,"copie pas initialisée ou de taille inférieure à original\n"), -1;
	for (i=0; i<original.taille; i++) copie->tab[i] = original.tab[i];
	return 1;
}

/* Si la taille d'un nombre est plus grande que necessaire, 
 * réalloue de la mémoire pour juste le nombre de bits qu'il faut */
void ajuste_taille(UINT_X *n)
{
	int i=n->taille-1, cmp=0;
	while(n->tab[i] == 0) { cmp++; i--; }
	if (cmp > 0)
	{
		n->taille -= cmp;
		uint64_t *temp = malloc(n->taille*sizeof(uint64_t));
		for(i=0; i<n->taille; i++) temp[i] = n->tab[i];
		free_uint_x(*n);
		n->tab=temp;
	}
}


/* Fait la somme en base 2 de deux variables de type uint64 */
uint64_t somme_uint64(uint64_t a,uint64_t b, char *finalRetenue){
	int i, temp=0;
	char retenue=0;
	uint64_t mask=1, resultat=0;
	
	if (b!=0 || *finalRetenue!=0){
		temp = access_bit_n(a,1) + access_bit_n(b,1) + *finalRetenue;
		*finalRetenue = 0;
		for(i=1;i<=64;i++){
			if(i!=1) temp = access_bit_n(a,i) + access_bit_n(b,i) + retenue;
		
			switch(temp){
			case 0:
				retenue = 0;
				break;
			case 1:
				resultat |= mask;
				retenue = 0;
				break;
			case 2:
				if(i==64) *finalRetenue = 1; 
				else retenue=1;
				break;
			default:
				if(i==64) *finalRetenue = 1; 
				else retenue = 1;
				resultat |= mask;
				break;
				}
			mask<<=1;
		}
	}
	else resultat = a;
	return resultat;
}

//attention, les calculs se font en base 2 donc la représentation en base 10 du nombre de type uint64 n'est pas correcte
//à voir pour créer une fonction permettant d'afficher ce nombre en base 10
//Fait la somme de deux nombres de type uint_x
void somme1(UINT_X *resultat, UINT_X a, UINT_X b)
{
	int i;
	char retenue=0;
	
	if(resultat->taille <= max(a.taille,b.taille)) {
		free_uint_x(*resultat);
		*resultat = malloc_uint_x((1+max(a.taille,b.taille))*64);
		}
	
	for (i=0; i<max(a.taille, b.taille); i++)
	{
		if (i < min(a.taille, b.taille))
		{
			resultat->tab[i] = somme_uint64(a.tab[i], b.tab[i], &retenue);
		}
		else 
		{
			if(max(a.taille, b.taille) == a.taille)
				resultat->tab[i] = somme_uint64(a.tab[i], 0, &retenue);
			else resultat->tab[i] = somme_uint64(b.tab[i], 0, &retenue);
		}
	}
	if(retenue==1) resultat->tab[max(a.taille,b.taille)]=1;
}

void incrementation(UINT_X* n);
void decrementation(UINT_X* n);

//Renvoie original shifté de bit+case_tab*64 dans la variable copie
//case_tab: le nombre de case à shifter
//bit: le nombre de bit à shifter dans la case après case_tab
//1er if: si le nombre de bit à shifter après case_tab = 0 ou que le shifte de bit a déjà été fait et qu'il n'y a plus de chiffre à décaler (temp.tab[i-1-case_tab]) alors le nombre est directement recpier dans copie
//sinon, on copie bit après bit le nombre de temp.tab[i] à partir du rang 64-bit
//temp.tab[i] est ensuite shifté de 64-bit et s'il est encore > 0,
//alors le reste est recopié lors de la boucle suivante dans la case de copie
//le dernier if sert à recopier les derniers bits dans la dernière case de copie
void shift(UINT_X *copie, UINT_X original, int case_tab, int bit)
{
	if(copie->tab != NULL) free_uint_x(*copie);
	*copie = malloc_uint_x((original.taille+case_tab+1)*64);
	UINT_X temp = malloc_uint_x(original.taille*64);
	int i,j;
	copier(&temp, original);
	
	for(i=case_tab;i<(temp.taille+case_tab);i++)
	{
		if(bit == 0 || (i>case_tab && temp.tab[i-1-case_tab]==0)) copie->tab[i]=temp.tab[i-case_tab];
		else
		{
			for(j=64-bit; j>0; j--)
			{
				copie->tab[i] <<=1;
				copie->tab[i] += access_bit_n(temp.tab[i-case_tab], j);
				
			}
			temp.tab[i-case_tab] >>= 64-bit;
			if(i==case_tab)	copie->tab[i] <<= bit;
			else
			{
				j=bit;
				while (j > 0)
				{				
					copie->tab[i] <<=1;
					copie->tab[i] += access_bit_n(temp.tab[i-1-case_tab], j);
					j--;
				}
			}
		}
	}
	if(temp.tab[i-1-case_tab] > 0)
	{
		j=bit;
		while (j > 0)
		{				
			copie->tab[i] <<=1;
			copie->tab[i] += access_bit_n(temp.tab[i-1-case_tab], j);
			j--;
		}
	}
	free_uint_x(temp);
}

void printf_binaire_uint64_t(uint64_t nb){
	uint64_t bit,mask;
	mask = 0x8000000000000000;
	int i;
		for(i=63;i>=0;--i){
			bit = (nb & mask) >> i;			
			printf("%"PRIu64,bit);
			mask >>= 1;
		}
		printf("\n");
}

//nb>>=1
void shift_droit(UINT_X* nb){
	uint64_t futur,temp;
	futur=temp=0;
	int i;
	for(i=(nb->taille)-1;i>=0;i--){
		futur=(uint64_t)access_bit_n(nb->tab[i],1);
		futur<<=((sizeof(uint64_t)*8)-1);
		nb->tab[i]>>=1;
		nb->tab[i]=(nb->tab[i])+temp;
		temp=futur;
		//printf_binaire_uint64_t(nb->tab[i]);
	}
}

//nb>>=x
void shift_droit_x(UINT_X* nb,int x){
	int i;
	for(i=0;i<x;i++){
		shift_droit(nb);
	}
}

uint64_t difference_uint64(uint64_t a,uint64_t b,int* finalRetenueInf){
	int i,temp;
	uint64_t mask,resultat;
	int retenueInf=0;
	mask=1;resultat=0;
	//printf("DEBUT\n");
	for(i=1;i<=64;i++){
		if(i==1){
			temp=gestion_retenue(a,b,i,&(*finalRetenueInf));
			
		}
		else {
			temp=gestion_retenue(a,b,i,&retenueInf);
		}
		//printf("%d",temp);
		if(temp==1) {
			resultat|=mask;
		}
		mask<<=1;
	}
	*finalRetenueInf=retenueInf;
	//printf("\n");
	return resultat;
}

UINT_X difference(UINT_X a,UINT_X b){ //ne marche pas 
	UINT_X resultat;
	int i,retenue;
	retenue=0;
	
	resultat=malloc_uint_x((max(a.taille,b.taille)+1)*64);
	for(i=0;i<(min(a.taille,b.taille));i++){
		//printf("A");
		resultat.tab[i]=difference_uint64(a.tab[i],b.tab[i],&retenue);
		//printf(">>>%
	}
	for(i=(min(a.taille,b.taille));i<max(a.taille,b.taille);i++){
		if(min(a.taille,b.taille)==(a.taille)){
			resultat.tab[i]=difference_uint64(b.tab[i],0,&retenue);
		}
		else resultat.tab[i]=difference_uint64(a.tab[i],0,&retenue);
		//printf("B\n");
	}
	if(retenue==1) resultat.tab[max(a.taille,b.taille)]=1;
	else resultat.tab[max(a.taille,b.taille)]=0;
	//ajuste_taille(&resultat);
	return resultat;
}


//nb<<=1
void shift_gauche(UINT_X* nb){
	/*
	 uint64_t futur,temp;
	temp=futur=0;
	int i;
	for(i=0;i<(nb->taille);i++){
		futur=(uint64_t)access_bit_n(nb->tab[i],(sizeof(uint64_t)*8));
		nb->tab[i]<<=1;
		nb->tab[i]=(nb->tab[i])+temp;
		temp=futur;
		//printf_binaire_uint64_t(nb->tab[i]);
	}*/
	uint64_t u=0x8000000000000000; 
	if((nb->tab[(nb->taille)-1] & u)==u){
		UINT_X copie=malloc_uint_x((nb->taille)*64+64);
		copier(&copie,*nb);
		free_uint_x(*nb);
		*nb=malloc_uint_x(copie.taille*64);
		copier(nb,copie);
		//nb->tab[(nb->taille)-1]=1;
		free_uint_x(copie);
	}
	uint64_t futur,temp;
	temp=futur=0;
	int i;
	for(i=0;i<(nb->taille);i++){ //enleve -1
		futur=(uint64_t)access_bit_n(nb->tab[i],(sizeof(uint64_t)*8));
		nb->tab[i]<<=1;
		nb->tab[i]=(nb->tab[i])+temp;
		temp=futur;
	}
}

//nb<<=x
void shift_gauche_x(UINT_X* nb,int x){
	int i;
	for(i=0;i<x;i++){
		shift_gauche(nb);
	}
}

//nb<<=x
void shift_gauche_uintx(UINT_X* nb,UINT_X x){
	
	UINT_X zero=malloc_uint_x(1*64);
	zero.tab[0]=0;
	int t=0;
	while(inferieur(zero,x)){
		t++;
		shift_gauche(nb);
		incrementation(&zero);
		ajuste_taille(&zero);
	}
	//printf("ITERATIONS %d\n",t);
	free_uint_x(zero);
}

void test(){
	int test=13;
	UINT_X decre=malloc_uint_x(1*64);
	UINT_X zero=malloc_uint_x(1*64);
	decre.tab[0]=3;
	zero.tab[0]=0;
	while(!inferieur_egal(decre,zero)){
		test--;
		decrementation(&decre);
	}
		
	printf(">>>%d\n",test);
		
}

//multiplication de a par b
//à chaque fois que le bit lu de b vaut 1, on décale a du nombre de bits égale au rang du bit lu de b
//puis on fait la somme avec le résultat précédent
void produit(UINT_X *resultat, UINT_X a, UINT_X b)
{
	UINT_X temp; temp.tab = NULL;
	UINT_X copie = malloc_uint_x(resultat->taille*64);
	int i,j;
	
	if (resultat->tab == NULL) *resultat = malloc_uint_x((a.taille + b.taille)*64);
	else if (resultat->taille < (a.taille + b.taille)){
		free_uint_x(*resultat);
		*resultat = malloc_uint_x((a.taille + b.taille)*64);
		}
	
	for(i=0; i<b.taille; i++)
	{
		for(j=1;j<=64;j++)
		{
			if (access_bit_n(b.tab[i], j))
			{
				copier(&copie, *resultat);
				shift(&temp,a,i,j-1);
				somme(resultat, copie, temp);
			}
		}
	}
	free_uint_x(temp);
	free_uint_x(copie);
	ajuste_taille(resultat);
}

int gestion_retenue(uint64_t a,uint64_t b,int i,int* retenueInf){
	int temp;
	if(*retenueInf==0){ //si il n'y a pas de retenue
		if((access_bit_n(a,i)==0)&&(access_bit_n(b,i)==0)){ // si la premiere colonne vaut 0 et 0
			temp=0; //PAS DE RETENUE DANS CE CAS
			*retenueInf=0; //peut etre a enlever
			//printf("A:0-0=0 sans retenue future\n");
		}
		else if((access_bit_n(a,i)==0)&&(access_bit_n(b,i)==1)){ //si ca vaut 0 et 1
			temp=1;
			*retenueInf=1;
			//printf("B:0-1=1 avec retenue future\n");
		}
		else if((access_bit_n(a,i)==1)&&(access_bit_n(b,i)==0)){ //si ca vaut 1 et 0
			temp=1;
			*retenueInf=0;
			//printf("C:1-0=1 sans retenue future\n");
		}
		else if((access_bit_n(a,i)==1)&&(access_bit_n(b,i)==1)){ //si ca vaut 1 et 1
			temp=0;
			*retenueInf=0;
			//printf("D:1-1=0 sans retenue future\n");
		}
	}
	else{ //si il y a une retenue finale
		if((access_bit_n(a,i)==0)&&(access_bit_n(b,i)==0)){ // si ca vaut 0 et 0 avec retenue inferieure
			temp=1;
			*retenueInf=1;
			//printf("E:0-0=1 avec retenue future\n");
		}
		else if((access_bit_n(a,i)==0)&&(access_bit_n(b,i)==1)){ //si ca vaut 0 et 1 avec retenue inferieure
			temp=0;
			*retenueInf=1;
			//printf("F:0-1=0 avec retenue future\n");
		}
		else if((access_bit_n(a,i)==1)&&(access_bit_n(b,i)==0)){ //si ca vaut 1 et 0 avec retenue inferieure
			temp=0;
			*retenueInf=0;
			//printf("G:1-0=0 sans retenue future\n");
		}
		else if((access_bit_n(a,i)==1)&&(access_bit_n(b,i)==1)){ //si ca vaut 1 et 1 avec retenue inferieure
			temp=1;
			*retenueInf=1;
			//printf("H:1-1=1 avec retenue future\n");
		}
	}
	//printf("%d",temp);
	return temp;
}





//renvoie 1 si a<b
int inferieur(UINT_X a,UINT_X b){
	int i;
	//ajuste_taille(&a);
	//ajuste_taille(&b);
	if(a.taille<b.taille) return 1;
	else if(a.taille==b.taille){
		for(i=a.taille-1;i>=0;i--){
			if(a.tab[i]<b.tab[i]) return 1;
			if(a.tab[i]>=b.tab[i]) return 0;
		}	
	}
	else if(a.taille>b.taille){
		for(i=a.taille-1;i>=b.taille-1;i--){
			if(a.tab[i]!=0) return 0;
		}
		for(i=b.taille-1;i>=0;i--){
			if(a.tab[i]<b.tab[i]) return 1;
			if(a.tab[i]>=b.tab[i]) return 0;
		}
		
	}
	return 2;
}

//renvoie 1 si a<b
int inferieur_egal(UINT_X a,UINT_X b){
	int i;
	//ajuste_taille(&a);
	//ajuste_taille(&b);
	if(a.taille<b.taille) return 1;
	else if(a.taille==b.taille){
		for(i=a.taille-1;i>=0;i--){
			if(a.tab[i]<b.tab[i]) return 1;
			if(a.tab[i]>b.tab[i]) return 0;
		}	
		return 1;
	}
	else if(a.taille>b.taille){
		for(i=a.taille-1;i>=b.taille-1;i--){
			if(a.tab[i]!=0) return 0;
		}
		for(i=b.taille-1;i>=0;i--){
			if(a.tab[i]<b.tab[i]) return 1;
			if(a.tab[i]>b.tab[i]) return 0;
		}
		return 1;
	}
	return 2;
}

/*void quotient(UINT_X a,UINT_X b,UINT_X* q){
	UINT_X n,p;
	n=malloc_uint_x(1*64);
	n.tab[0]=0; //n=0
	if(inferieur(a,b)){
		UINT_X zero;
		zero=malloc_uint_x(1*64);
		zero.tab[0]=0; //n=0
	}
	p=malloc_uint_x(b.taille*64);
	copier(&p,b);
	//return p;
	
}*/

void incrementation(UINT_X* n){
	UINT_X un;
	un=malloc_uint_x(1*64);
	un.tab[0]=1;
	int taille=n->taille;
	UINT_X temp=malloc_uint_x((taille+1)*64);
	somme(&temp,*n,un);
	free_uint_x(un);
	free_uint_x(*n);
	//realocation + copie
	*n=malloc_uint_x(temp.taille*64);
	copier(n,temp);
	free_uint_x(temp);
	//ajuste_taille(n); //peut etre a remettre si incrementation ne marche pas
}

void decrementation(UINT_X* n){
	ajuste_taille(n); //peut etre a remettre si decrementation ne marche plus
	uint64_t mask=MAX_UINT64-1;
	uint64_t test=n->tab[0];
	if(access_bit_n(test,1)==1){
		n->tab[0]&=mask;
		//printf("A");
		//ajuste_taille(n);
	}
	else{ //ici cela pose probleme
		//printf("B");
		UINT_X deux;
		deux=malloc_uint_x(1*64);
		deux.tab[0]=2;
		//int taille=n->taille;
		UINT_X temp;
		temp=difference(*n,deux);
		//printf("temp %d\n",temp.taille);
		//printf("n %d\n",n->taille);
		incrementation(&temp);
		free_uint_x(*n);
		//reallocation+copie
		*n=malloc_uint_x((temp.taille)*64);
		copier(n,temp);
		free_uint_x(temp);
		free_uint_x(deux);
		ajuste_taille(n);
	}
	
}



UINT_X quotient(UINT_X a,UINT_X b){
	UINT_X n,p,q_copie;
	ajuste_taille(&a);
	ajuste_taille(&b);
	if(inferieur(a,b)){ //si a<b
		UINT_X zero;
		zero=malloc_uint_x(1*64);
		zero.tab[0]=0; //n=0
		return zero; //return 0
	}
	n=malloc_uint_x(1*64);
	n.tab[0]=0; //n=0
	p=malloc_uint_x(b.taille*64);
	copier(&p,b); //p=b
	
	ajuste_taille(&a);
	//int compteur=0;
	while(inferieur_egal(p,a)){ //while p<=a
		shift_gauche(&p); //p<<=1
		incrementation(&n); //n++
		//compteur++;
	}
	/*printf("COMPTEUR=%d ETAPE 1: p= \n",compteur);
	printf_binaire_uint_x(p);
	printf(" OK\n"); */
	shift_droit(&p); //p>>=1
	/*printf("ETAPE 2: p= ");
	printf_binaire_uint_x(p);
	printf(" OK\n");*/
	decrementation(&n); //n-- 
	/*printf("ETAPE 3: n= ");
	printf_binaire_uint_x(n);
	printf(" OK\n");*/
	
	q_copie=malloc_uint_x(1*64);
	q_copie.tab[0]=1; //q_copie=1
	shift_gauche_uintx(&q_copie,n); //q_copie=(1<<n); 
	/*printf("ETAPE 4: q_copie= \n");
	printf_binaire_uint_x(q_copie);
	printf(" OK\n");*/


	UINT_X aux;
	UINT_X aux_temp = malloc_uint_x(p.taille*64);
	copier(&aux_temp,p); //aux=p
	/*printf("ETAPE 5: aux_temp= \n");
	printf_binaire_uint_x(aux_temp);
	printf(" OK\n");*/
	
	UINT_X zero_cond,somme_aux_p;
	zero_cond=malloc_uint_x(1*64);
	zero_cond.tab[0]=0;
	
	int compteur1=0;
	int compteur2=0;
	while(inferieur(zero_cond,n)){ //while n>0
		compteur1++;
		shift_droit(&p); //p>>=1
		decrementation(&n); //n--
		
		somme_aux_p=malloc_uint_x((max(aux_temp.taille,p.taille)+1)*64);
		somme(&somme_aux_p,aux_temp,p); //somme_aux_p = aux + p
		ajuste_taille(&somme_aux_p);
		
		if(inferieur_egal(somme_aux_p,a)){ //if (aux+p)<=a
			compteur2++;
			////////q+=(1<<n)
			UINT_X un_shift=malloc_uint_x(1*64);
			un_shift.tab[0]=1; //un_shift=1
			shift_gauche_uintx(&un_shift,n); //un_shift=(1<<n); 
			UINT_X q=malloc_uint_x((max(q_copie.taille,un_shift.taille)+1)*64);
			somme(&q,q_copie,un_shift);
			free_uint_x(q_copie);
			ajuste_taille(&q);
			q_copie=malloc_uint_x(q.taille*64);
			copier(&q_copie,q);
			free_uint_x(q);
			free_uint_x(un_shift);
			/////////
			/////////aux+=p
			aux=malloc_uint_x((max(aux_temp.taille,p.taille)+1)*64);
			somme(&aux,aux_temp,p);
			ajuste_taille(&aux);
			free_uint_x(aux_temp);
			aux_temp=malloc_uint_x(aux.taille*64);
			copier(&aux_temp,aux);
			free_uint_x(aux);
			/////////
			//ici pr le modulo rajouter aux+=p
			//break; //je ne sais pas si il faut lenlever
		}
		free_uint_x(somme_aux_p);
	}

	/*printf("ETAPE 6: n= \n");
	printf_binaire_uint_x(n);
	printf(" OK\n");
	
	printf("ETAPE 7: p= \n");
	printf_binaire_uint_x(p);
	printf(" OK\n");*/
	
	printf("COMPTEUR1: %d ,COMPTEUR2: %d ETAPE 8: aux_temp= \n",compteur1,compteur2);
	printf_binaire_uint_x(aux_temp);
	printf("\n");
	//printf(" OK\n");

	/*printf("ETAPE 9: q= \n");
	printf_binaire_uint_x(q_copie);
	printf(" OK\n");*/
	
	free_uint_x(n);
	free_uint_x(p);
	free_uint_x(aux_temp);
	free_uint_x(zero_cond);
	
	return q_copie;
		
}

/////////***********************FONCTIONS AMELIOREES*******************///////////

/* Il faut que resultat soit initialisé resultat->taille >= max(a.taille, b.taille) + 1.
 * Return value : on success return 1, on failure return -1
 * 
 * Lors du calcul de la retenue avec la valeur du bit de poid fort de resultat
 * car si la retenue est > 4, la vraie valeur de la retenue est difficile à avoir 
 * donc on fait retenue + 1 puis on shift pour avoir sa bonne valeur 
 * de meme pour les retenue paires, ret de 10 vaut en fait 1 pour le bloc suivant 
 * exemple: res = 10001 et ret = 101
 * --> res = 00001 et ret = 101+1 = 110 et non ret = 101-10 
 * (10 n'étant pas la même valeur à chaque fois)
 * --> res = 11 00001 */
int somme (UINT_X *resultat, UINT_X a, UINT_X b){
	if(resultat->tab == NULL || resultat->taille <= max(a.taille, b.taille))
		return fprintf(stderr, "variable resultat non initialisee ou la taille est inférieure à max(a.taille, b.taille) + 1\n"), -1;
	
	char retenue = 0; 
	int i;
	
	/* Copie de a et b */
	UINT_X copieA = malloc_uint_x(max(a.taille, b.taille)*64);
	UINT_X copieB = malloc_uint_x(max(a.taille, b.taille)*64);
	if(copier(&copieA, a) == -1) return -1;
	if(copier(&copieB, b) == -1) return -1;
	
	
	
	for(i=0; i < copieA.taille; i++){
		/* on ajoute la retenue dans a ou b s'ils sont < MAX_UINT64 
		 * sinon on garde la retenue
		 * la retenue est shifté vers la gauche car le bit de poids faible correcpond
		 * au bit de poids fort du bloc précédent. */
		if((retenue > 0) && (copieA.tab[i] < MAX_UINT64)) {
			copieA.tab[i] += retenue>>1;
			retenue = 0;
		}
		else if((retenue > 0) && (copieB.tab[i] < MAX_UINT64)) {
			copieB.tab[i] += retenue;
			retenue = 0;
		}
		/* on récupère les deux bits de poids fort */
		retenue = access_bit_n(copieA.tab[i], 64) + access_bit_n(copieB.tab[i], 64);
		/* on met les deux bits de poids fort à zéro */
		copieA.tab[i] <<= 1; copieA.tab[i] >>= 1;
		copieB.tab[i] <<= 1; copieB.tab[i] >>= 1;
		resultat->tab[i] = copieA.tab[i] + copieB.tab[i];
		
		/* CAS : retenue paire --> Rien
		 * CAS : retenue impaire -->
		 ** si le bit de poids fort de resultat.tab[i]==0, le bit est mis à 1 
		 * et la retenue diminue de 1
		 ** si le bit de poids fort de resultat.tab[i]==1, le bit est mis à zéro 
		 * et la retenue augmente de 1 */
		if(retenue%2){
			if(access_bit_n(resultat->tab[i], 64) == 0){
				resultat->tab[i] |= 0x8000000000000000;
				retenue--;
			}
			else {
				retenue++;
				resultat->tab[i] <<= 1; resultat->tab[i] >>= 1;
			}
		}
	}
	/* s'il reste une retenue, elle est ajoutée au dernier bloc */
	resultat->tab[i] += retenue>>1;
	free_uint_x(copieA);
	free_uint_x(copieB);	
	
	return 0;
} 

//affiche un nombre de type uint_x en binaire
void printf_binaire_uint_x(UINT_X n)
{
	int i,j; 
	uint64_t bit=0, mask;
	for(j=n.taille-1;j>=0;j--){
		mask = 0x8000000000000000;
		for(i=63;i>=0;--i){
			bit = (n.tab[j] & mask) >> i;			
			printf("%"PRIu64,bit);
			mask >>= 1;
		}
		//printf("\n");
	}
}

void fprintf_binaire_uint_x(FILE* f,UINT_X n){
	int i,j; 
	uint64_t bit=0, mask;
	for(j=n.taille-1;j>=0;j--){
		mask = 0x8000000000000000;
		for(i=63;i>=0;--i){
			bit = (n.tab[j] & mask) >> i;			
			fprintf(f,"%"PRIu64,bit);
			mask >>= 1;
		}
		//fprintf(f,"\n");
	}
}

void alea_uint_x(UINT_X* u){
	int i;
	for(i=0;i<(u->taille);i++){
		u->tab[i]=(rand()*rand())%MAX_UINT64;
		//u->tab[i]=MAX_UINT64;
	}
}

int main(){
	FILE* f=fopen("test","w");
	srand(time(NULL));
	UINT_X nb,nb2,resultat;
	nb=malloc_uint_x(512);
	alea_uint_x(&nb);
	//nb.tab[0]=150;
	ajuste_taille(&nb);
	printf_binaire_uint_x(nb);
	printf("\n/\n");
	fprintf_binaire_uint_x(f,nb);
	
	fprintf(f,"\n");
	nb2=malloc_uint_x(512);
	alea_uint_x(&nb2);
	//nb2.tab[0]=8;
	ajuste_taille(&nb2); //necessaire jai limpression
	printf_binaire_uint_x(nb2);
	printf("\n=\n");
	fprintf_binaire_uint_x(f,nb2);
	fprintf(f,"\n=");
	resultat=quotient(nb,nb2);
	//printf("\nRESULTAT : \n");
	//printf_binaire_uint_x(resultat);
	fprintf_binaire_uint_x(f,resultat);
	
	free_uint_x(nb);
	free_uint_x(nb2);
	free_uint_x(resultat);
	fclose(f);
	
	/*UINT_X n=malloc_uint_x(64);
	n.tab[0]=1;
	UINT_X y=malloc_uint_x(64);
	y.tab[0]=4;
	shift_gauche_uintx(&n,y);
	printf_binaire_uint_x(n);
	free_uint_x(n);
	free_uint_x(y);*/
	
	return 0;
}
