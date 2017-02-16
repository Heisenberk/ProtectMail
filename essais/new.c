#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h> 

#define NB_CAR_UINT64 20
#define MAX_UINT64 0xFFFFFFFFFFFFFFFF
#define max(a,b) ((a<b)?(b):(a))
#define min(a,b) ((a<b)?(a):(b))

struct uint_x{
	int taille;
	uint64_t* tab;
}; typedef struct uint_x UINT_X;

UINT_X init_uint_x2(int taille){
	int i;
	UINT_X new;
	new.taille=taille/(8*sizeof(uint64_t));
	new.tab=malloc(new.taille*sizeof(uint64_t));
	if(new.tab==NULL){
		printf("IMPOSSIBLE DE RESERVER DE LA MEMOIRE POUR UINT_X\n");
		exit(1);
	}
	for(i=0;i<new.taille;i++){
		new.tab[i]=5678;
		//printf(">%d\n",i);
	}
	return new;
}

UINT_X init_uint_x(int taille){
	int i;
	UINT_X new;
	new.taille=taille/(8*sizeof(uint64_t));
	new.tab=malloc(new.taille*sizeof(uint64_t));
	if(new.tab==NULL){
		printf("IMPOSSIBLE DE RESERVER DE LA MEMOIRE POUR UINT_X\n");
		exit(1);
	}
	for(i=0;i<new.taille;i++){
		new.tab[i]=678;
		//printf(">%d\n",i);
	}
	return new;
}

void free_uint_x(UINT_X n){
	free(n.tab);
}

int access_bit_n(uint64_t in,int n){
	if(n<=0) return -1;
	else if(n>64) return -1;
	in>>=(n-1);
	in<<=(64-1);
	in>>=(64-1);
	return in;
}

void printf_uint_x (UINT_X n){
	int i,j;uint64_t temp;
	for(j=n.taille-1;j>=0;j--){
		for(i=64;i>0;i--){
			temp=n.tab[j];
			temp>>=(i-1);
			temp<<=(64-1);
			temp>>=(64-1);
			printf("%"PRIu64,temp);
		}
		printf("\n");
	}
}

uint64_t somme_uint64(uint64_t a,uint64_t b,int* finalRetenue){
	//int finalRetenue=0;
	int i,y,temp;
	int retenue=0;
	uint64_t resultat,mask;
	mask=1;
	resultat=0;
	for(i=1;i<=64;i++){
		if(i==1) temp=access_bit_n(a,i)+access_bit_n(b,i)+(*finalRetenue);
		else temp=access_bit_n(a,i)+access_bit_n(b,i)+retenue;
		//retenue=0;
		//printf("%d+%d+%d=%d(TEMP)\n",access_bit_n(a,i),access_bit_n(b,i),retenue,temp);
		if(temp==0){
			retenue=0;
			//printf("0");
		}
		else if(temp==1) {
			resultat|=mask;
			retenue=0;
			//printf("1");
		}
		else if(temp==2){
			if(i==64) *finalRetenue=1; 
			else retenue=1;
			//else retenue+=(calcul*2);
			//else retenue==1;
			//printf("2");
		}
		else {
			if(i==64) *finalRetenue=1; 
			else retenue=1;
			resultat|=mask;
			//else retenue=1;
			//retenue+=calcul;
			//printf("3");
		}
		//calcul*=2;
		mask<<=1;
		
	}
	//printf("RETENUE FINALE: %d\n",finalRetenue);
	/*printf("\n");
	printf("A: ");
	for(i=64;i>0;i--){
		printf("%d",access_bit_n(a,i));
	}
	printf("\n");
	printf("B: ");
	for(i=64;i>0;i--){
		printf("%d",access_bit_n(b,i));
	}
	printf("\n");
	printf("C: ");
	for(i=64;i>0;i--){
		printf("%d",access_bit_n(resultat,i));
	}*/
	/*printf("\n");
	printf("RETENUE FINALE: %d\n",finalRetenue);*/
	return resultat;
}

UINT_X somme (UINT_X a,UINT_X b){
	UINT_X resultat;
	int i,retenue;
	retenue=0;
	
	resultat=init_uint_x((max(a.taille,b.taille)+1)*64);
	for(i=0;i<(min(a.taille,b.taille));i++){
		resultat.tab[i]=somme_uint64(a.tab[i],b.tab[i],&retenue);
	}
	for(i=(min(a.taille,b.taille));i<max(a.taille,b.taille);i++){
		if(min(a.taille,b.taille)==(a.taille)){
			resultat.tab[i]=somme_uint64(b.tab[i],0,&retenue);
		}
		else resultat.tab[i]=somme_uint64(a.tab[i],0,&retenue);
	}
	if(retenue==1) resultat.tab[max(a.taille,b.taille)]=1;
	else resultat.tab[max(a.taille,b.taille)]=0;
	return resultat;
}

int main(){
	UINT_X n=init_uint_x(512);
	UINT_X m=init_uint_x2(1024);
	//UINT_X u=init_uint_x(512);
	//printf(">%d\n",n.taille);
	printf_uint_x(n);
	printf("\n");
	printf_uint_x(m);
	//printf_uint_x(u);
	
	printf("\n");
	UINT_X resultat=somme(n,m);
	//printf("%d\n",resultat.taille);
	printf_uint_x(resultat);
	printf("\n");
	free_uint_x(n);
	free_uint_x(m);
	free_uint_x(resultat);

	/*uint64_t a,b,c;
	a=MAX_UINT64;
	b=69;
	int i;
	//printf("\n");
	printf("\n");
	int pointeur=0;
	c=somme_uint64(a,b,&pointeur);
	printf("\n");
	printf("%"PRIu64,c);
	//printf(">%d\n",pointeur);
	printf("\n");*/
	
	//printf("%d\n",min(3,2));
	return 0;
}
