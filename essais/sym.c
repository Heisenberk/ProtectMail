#include <gmp.h>
#include <stdio.h>
#include <stdlib.h>

//A REGLER ACCENT!!!
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

void encrypt(char* nomFichier1,char* nomFichier2,char* cle){
	FILE* f1=fopen(nomFichier1,"r");
	FILE* f2=fopen(nomFichier2,"w");
	int k=0;
	char c;
	do{
		c=fgetc(f1);
		if(c!=EOF){
			/*////
			if(c<0) fprintf(f2,"-%d ",xor(cle[k%16],-c));
			////
			else*/ fprintf(f2,"%d ", xor(cle[k%16],c));
			k++;
		}
	}while(c!=EOF);
	fclose(f1);
	fclose(f2);
}

void decrypt(char* nomFichier1,char* nomFichier2,char* cle){
	FILE* f1=fopen(nomFichier1,"r");
	FILE* f2=fopen(nomFichier2,"w");
	int c,k=0;
	//int c;
	char car;
	do{
		car=fgetc(f1);
		if(isdigit(car)){
			ungetc(car,f1);
			fscanf(f1,"%d",&c);
			/*//
			if(c<0){
				int temp=xor(cle[k%16],-c);
				fprintf(f2,"%c",-temp);
			}
			//
			else */fprintf(f2,"%c",xor(cle[k%16],c));
			k++;
		}
	}while(car!=EOF);
	fclose(f1);
	fclose(f2);
}

int main(){
	gmp_randstate_t state;
	gmp_randinit_default (state);
	gmp_randseed_ui (state, (unsigned) time(NULL));

	char cleSession[17];
	cleSession[16]='\0';
	int i;
	for(i=0;i<16;i++){
		cleSession[i]=random_int(state);
	}
	printf("CLEF DE SESSION : %s\n",cleSession);
	
	encrypt("texte","chiffre",cleSession);
	decrypt("chiffre","dechiffre",cleSession);
	gmp_randclear(state);
	
	return 0;
}
