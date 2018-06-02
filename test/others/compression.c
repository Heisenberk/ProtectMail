#include <stdio.h>
#include <stdlib.h>
//ON NE CONSIDERE PAS LES ACCENTS

int compte_car_fichier(char* nomFichier){
	FILE* f=fopen(nomFichier,"r");
	int compteur=0;
	char car;
	do{
		car=fgetc(f);
		if(car!=EOF){
			//printf("'%c'->%d\n",car,car);
			compteur++;
		}
	}while(car!=EOF);
	fclose(f);
	return compteur;
}

void remplit_tableau_vide(int* T,int n){
	int i;
	for(i=0;i<n;i++){
		T[i]=0;
	}
}

void affiche_tableau(int* T,int n){
	int i;
	for(i=0;i<n;i++){
		printf("%d\n",T[i]);
	}
}

void remplit_frequences(int* T,char* nomFichier){
	FILE* f=fopen(nomFichier,"r");
	char car;
	int ascii;
	do{
		car=fgetc(f);
		ascii=car;
		if(car!=EOF){
			T[ascii]=T[ascii]+1;
			//printf("'%c'->%d\n",car,car);
			//compteur++;
		}
	}while(car!=EOF);
	fclose(f);
	//return compteur;
}

int main(){
	printf("%d\n",compte_car_fichier("test"));
	int freq[128];
	remplit_tableau_vide(freq,128);
	remplit_frequences(freq,"test");
	affiche_tableau(freq,128);
	return 0;
}
