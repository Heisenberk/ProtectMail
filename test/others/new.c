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
		new.tab[i]=678; //METTRE 0 NORMALEMENT
	}
	return new;
}

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
		new.tab[i]=66789939; //METTRE 0 NORMALEMENT
	}
	return new;
}

void free_uint_x(UINT_X n){
	free(n.tab);
}

//si la taille d'un nombre est plus grande que necessaire, 
//réalloue de la mémoire pour juste le nombre de bits qu'il faut
void ajuste_taille(UINT_X *n)
{
	int i=(*n).taille-1, cmp=0;
	while((*n).tab[i] == 0) { cmp++; i--; }
	//printf("cmp : %d\n", cmp);
	if (cmp > 0)
	{
		(*n).taille -= cmp;
		uint64_t *temp = malloc((*n).taille*sizeof(uint64_t));
		for(i=0; i<(*n).taille; i++) temp[i] = (*n).tab[i];
		free_uint_x(*n);
		(*n).tab=temp;
	}
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

void printf_uint64_binaire (uint64_t n){
	int i,j;uint64_t temp;
	//for(j=n.taille-1;j>=0;j--){
		for(i=64;i>0;i--){
			temp=n;
			temp>>=(i-1);
			temp<<=(64-1);
			temp>>=(64-1);
			printf("%"PRIu64,temp);
		}
		printf("\n");
	//}
}

uint64_t somme_uint64_bak(uint64_t a,uint64_t b, char *finalRetenue){
	int i, temp;
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



uint64_t somme_uint64_cms(uint64_t a,uint64_t b,int* finalRetenue){
	int i,y,temp;
	int retenue=0;
	uint64_t resultat,mask;
	mask=1;
	resultat=0;
	for(i=1;i<=64;i++){
		if(i==1){
			temp=access_bit_n(a,i)+access_bit_n(b,i)+(*finalRetenue);
			*finalRetenue=0;
		}
		else temp=access_bit_n(a,i)+access_bit_n(b,i)+retenue;
		if(temp==0){
			retenue=0;
		}
		else if(temp==1) {
			resultat|=mask;
			retenue=0;
		}
		else if(temp==2){
			if(i==64) *finalRetenue=1; 
			else retenue=1;
		}
		else {
			if(i==64) *finalRetenue=1; 
			else retenue=1;
			resultat|=mask;
		}
		mask<<=1;
		
	}
	return resultat;
}

UINT_X somme_bak(UINT_X resultat, UINT_X a, UINT_X b)
{
	int i;
	char retenue;
	
	for (i=0; i<max(a.taille, b.taille); i++)
	{
		if (i < min(a.taille, b.taille))
		{
			resultat.tab[i] = somme_uint64_bak(a.tab[i], b.tab[i], &retenue);
		}
		else 
		{
			if(max(a.taille, b.taille) == a.taille)
				resultat.tab[i] = somme_uint64_bak(a.tab[i], 0, &retenue);
			else resultat.tab[i] = somme_uint64_bak(b.tab[i], 0, &retenue);
		}
	}
	if(retenue==1) resultat.tab[max(a.taille,b.taille)]=1;
	return resultat;
}

UINT_X somme_cms(UINT_X a,UINT_X b){
	UINT_X resultat;
	int i,retenue;
	retenue=0;
	
	resultat=init_uint_x((max(a.taille,b.taille)+1)*64);
	for(i=0;i<(min(a.taille,b.taille));i++){
		resultat.tab[i]=somme_uint64_cms(a.tab[i],b.tab[i],&retenue);
	}
	for(i=(min(a.taille,b.taille));i<max(a.taille,b.taille);i++){
		if(min(a.taille,b.taille)==(a.taille)){
			resultat.tab[i]=somme_uint64_cms(b.tab[i],0,&retenue);
		}
		else resultat.tab[i]=somme_uint64_cms(a.tab[i],0,&retenue);
	}
	if(retenue==1) resultat.tab[max(a.taille,b.taille)]=1;
	else resultat.tab[max(a.taille,b.taille)]=0;
	ajuste_taille(&resultat);
	return resultat;
}

/*uint64_t difference_uint64(uint64_t a,uint64_t b,int* finalRetenueInf){
	int i,temp;
	uint64_t mask,resultat;
	int retenueInf=0;
	mask=1;resultat=0;
	for(i=1;i<=64;i++){
		if(i==1){
			if(*finalRetenueInf==0){ //si il n'y a pas de retenue
				if((access_bit_n(a,i)==0)&&(access_bit_n(b,i))){ // si la premiere colonne vaut 0 et 0
					temp=0;
					//PAS DE RETENUE
				}
				else if((access_bit_n(a,i)==0)&&(access_bit_n(b,i)==1)){ //si ca vaut 0 et 1
					temp=1;
					retenueInf=1;
				}
				else if((access_bit_n(a,i)==1)&&(access_bit_n(b,i)==0)){ //si ca vaut 1 et 0
					temp=1;
					retenueInf=0;
				}
				else if((access_bit_n(a,i)==1)&&(access_bit_n(b,i)==1)){ //si ca vaut 1 et 1
					temp=0;
					retenueInf=0;
				}
			}
			else{ //si il y a une retenue finale
				if((access_bit_n(a,i)==0)&&(access_bit_n(b,i)==0)){ // si ca vaut 0 et 0 avec retenue inferieure
					temp=1;
					retenueInf=1;
				}
				else if((access_bit_n(a,i)==0)&&(access_bit_n(b,i)==1)){ //si ca vaut 0 et 1 avec retenue inferieure
					temp=0;
					retenueInf=1;
				}
				else if((access_bit_n(a,i)==1)&&(access_bit_n(b,i)==0)){ //si ca vaut 1 et 0 avec retenue inferieure
					temp=0;
					retenueInf=0;
				}
				else if((access_bit_n(a,i)==1)&&(access_bit_n(b,i)==1)){ //si ca vaut 1 et 1 avec retenue inferieure
					temp=1;
					retenueInf=1;
				}
			}
			
		}
		//FIN ICI
		else {
			if(retenueInf==0){ //si il n'y a pas de retenue
				if((access_bit_n(a,i)==0)&&(access_bit_n(b,i)==0)){ // si la premiere colonne vaut 0 et 0
					temp=0;
					//PAS DE RETENUE
				}
				else if((access_bit_n(a,i)==0)&&(access_bit_n(b,i)==1)){ //si ca vaut 0 et 1
					temp=1;
					retenueInf=1;
				}
				else if((access_bit_n(a,i)==1)&&(access_bit_n(b,i)==0)){ //si ca vaut 1 et 0
					temp=1;
					retenueInf=0;
				}
				else if((access_bit_n(a,i)==1)&&(access_bit_n(b,i)==1)){ //si ca vaut 1 et 1
					temp=0;
					retenueInf=0;
				}
			}
			else{ //si il y a une retenue finale
				if((access_bit_n(a,i)==0)&&(access_bit_n(b,i)==0)){ // si ca vaut 0 et 0 avec retenue inferieure
					temp=1;
					retenueInf=1;
				}
				else if((access_bit_n(a,i)==0)&&(access_bit_n(b,i)==1)){ //si ca vaut 0 et 1 avec retenue inferieure
					temp=0;
					retenueInf=1;
				}
				else if((access_bit_n(a,i)==1)&&(access_bit_n(b,i)==0)){ //si ca vaut 1 et 0 avec retenue inferieure
					temp=0;
					retenueInf=0;
				}
				else if((access_bit_n(a,i)==1)&&(access_bit_n(b,i)==1)){ //si ca vaut 1 et 1 avec retenue inferieure
					temp=1;
					retenueInf=1;
				}
			}
		}
		//printf("%d",temp);
		if(temp==1) {
			resultat|=mask;
			//retenue=0;
		}
		//BIZARRE
		//
		mask<<=1;
		//printf_uint64_binaire(mask);
	}
	*finalRetenueInf=retenueInf;
	printf("\n");
	return resultat;
}*/

int gestion_retenue(uint64_t a,uint64_t b,int i,int* retenueInf){
	int temp;
	if(*retenueInf==0){ //si il n'y a pas de retenue
		if((access_bit_n(a,i)==0)&&(access_bit_n(b,i)==0)){ // si la premiere colonne vaut 0 et 0
			temp=0; //PAS DE RETENUE DANS CE CAS
		}
		else if((access_bit_n(a,i)==0)&&(access_bit_n(b,i)==1)){ //si ca vaut 0 et 1
			temp=1;
			*retenueInf=1;
		}
		else if((access_bit_n(a,i)==1)&&(access_bit_n(b,i)==0)){ //si ca vaut 1 et 0
			temp=1;
			*retenueInf=0;
		}
		else if((access_bit_n(a,i)==1)&&(access_bit_n(b,i)==1)){ //si ca vaut 1 et 1
			temp=0;
			*retenueInf=0;
		}
	}
	else{ //si il y a une retenue finale
		if((access_bit_n(a,i)==0)&&(access_bit_n(b,i)==0)){ // si ca vaut 0 et 0 avec retenue inferieure
			temp=1;
			*retenueInf=1;
		}
		else if((access_bit_n(a,i)==0)&&(access_bit_n(b,i)==1)){ //si ca vaut 0 et 1 avec retenue inferieure
			temp=0;
			*retenueInf=1;
		}
		else if((access_bit_n(a,i)==1)&&(access_bit_n(b,i)==0)){ //si ca vaut 1 et 0 avec retenue inferieure
			temp=0;
			*retenueInf=0;
		}
		else if((access_bit_n(a,i)==1)&&(access_bit_n(b,i)==1)){ //si ca vaut 1 et 1 avec retenue inferieure
			temp=1;
			*retenueInf=1;
		}
	}
	return temp;
}

uint64_t difference_uint64(uint64_t a,uint64_t b,int* finalRetenueInf){
	int i,temp;
	uint64_t mask,resultat;
	int retenueInf=0;
	mask=1;resultat=0;
	for(i=1;i<=64;i++){
		if(i==1){
			temp=gestion_retenue(a,b,i,&(*finalRetenueInf));
			
		}
		else {
			temp=gestion_retenue(a,b,i,&retenueInf);
		}
		if(temp==1) {
			resultat|=mask;
		}
		mask<<=1;
	}
	*finalRetenueInf=retenueInf;
	//printf("\n");
	return resultat;
}

UINT_X difference(UINT_X a,UINT_X b){
	UINT_X resultat;
	int i,retenue;
	retenue=0;
	
	resultat=init_uint_x((max(a.taille,b.taille)+1)*64);
	for(i=0;i<(min(a.taille,b.taille));i++){
		resultat.tab[i]=difference_uint64(a.tab[i],b.tab[i],&retenue);
	}
	for(i=(min(a.taille,b.taille));i<max(a.taille,b.taille);i++){
		if(min(a.taille,b.taille)==(a.taille)){
			resultat.tab[i]=difference_uint64(b.tab[i],0,&retenue);
		}
		else resultat.tab[i]=difference_uint64(a.tab[i],0,&retenue);
	}
	if(retenue==1) resultat.tab[max(a.taille,b.taille)]=1;
	else resultat.tab[max(a.taille,b.taille)]=0;
	ajuste_taille(&resultat);
	return resultat;
}

//PENSER A FAIRE UNE FONCTION QUI COMPARE DEUX UINTX
int main(){
	UINT_X n=init_uint_x2(512);
	UINT_X m=init_uint_x(1024);
	printf("A\n");
	printf_uint_x(n);
	printf("\nB\n");
	printf_uint_x(m);
	
	printf("\n");
	UINT_X resultat=difference(m,n);
	printf_uint_x(resultat);
	//printf(">%d\n",resultat.taille);
	printf("\n");
	free_uint_x(n);
	free_uint_x(m);
	//free_uint_x(resultat);
	/*uint64_t a,b,c;
	a=12;
	b=2;
	int retenue=0;
	c=difference_uint64(a,b,&retenue);
	printf_uint64_binaire(a);
	printf_uint64_binaire(b);
	printf_uint64_binaire(c);
	printf("%"PRIu64,c);
	printf("\n");
	printf("RETENUE:%d\n",retenue);
	//printf("%"PRIu64,c);
	printf("\n");*/
	return 0;
}

