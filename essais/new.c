#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h> 

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
		new.tab[i]=0;
		//printf(">%d\n",i);
	}
	/*for(i=0;i<new.taille;i++){
		printf("%"PRIu64, new.tab[i]);

	}*/
	return new;
}

void free_uint_x(UINT_X n){
	free(n.tab);
}

void printf_uint_x(UINT_X n){
	int i=n.taille-1; //TAILLE MAX
	//printf("!%d\n",n.taille);
	/*for(i=0;i<n.taille;i++){
		printf("%"PRIu64, n.tab[i]);
	}*/
	while(n.tab[i]==0){
		//printf(":%d\n",i);
		i--;
		if(i<0){
			printf("0");
			break;
		}
		//printf("%d\n",i);
	}
	//printf("w%d\n",i);
	while(i>=0){
		
		printf("%"PRIu64, n.tab[i]);
		i--;
	}
	//printf("J\n");
}

int main(){
	UINT_X n=init_uint_x(512);
	printf_uint_x(n);
	printf("\n");
	free_uint_x(n);
	return 0;
}
