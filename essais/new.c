#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h> 

#define NB_CAR_UINT64 20
#define MAX_UINT64 0xFFFFFFFFFFFFFFFF

struct uint_x{
	int taille;
	uint64_t* tab;
}; typedef struct uint_x UINT_X;

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
		new.tab[i]=1678;
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

uint64_t somme_uint64(uint64_t a,uint64_t b){
	int finalRetenue=0;
	int i,y,temp;
	int retenue=0;
	uint64_t resultat,mask;
	mask=1;
	resultat=0;
	for(i=1;i<=64;i++){
		if(i==1) temp=access_bit_n(a,i)+access_bit_n(b,i)+finalRetenue;
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
			if(i==64) finalRetenue=1; 
			else retenue=1;
			//else retenue+=(calcul*2);
			//else retenue==1;
			//printf("2");
		}
		else {
			if(i==64) finalRetenue=1; 
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
	printf("\n");
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
	}
	printf("\n");
	printf("RETENUE FINALE: %d\n",finalRetenue);
	return resultat;
}

int main(){
	/*UINT_X n=init_uint_x(512);
	//printf(">%d\n",n.taille);
	printf_uint_x(n);
	printf("\n");
	free_uint_x(n);*/

	uint64_t a,b,c;
	a=0xFFFFFFFFFFFFFFFF;
	b=69;
	int i;
	//printf("\n");
	/*for(i=64;i>0;i--){
		printf("%d",access_bit_n(a,i));
	}
	a+=4;
	printf("\n");
	for(i=64;i>0;i--){
		printf("%d",access_bit_n(a,i));
	}*/
	printf("\n");
	c=somme_uint64(a,b);
	printf("%"PRIu64,c);
	printf("\n");
	return 0;
}
