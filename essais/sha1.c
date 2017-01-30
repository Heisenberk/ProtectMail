#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include <string.h>

#define TAILLE_OCTET (8)

#define K1 (0x5A827999)
#define K2 (0x6ED9EBA1)
#define K3 (0x8F1BBCDC)
#define K4 (0xCA62C1D6)

// -lm compilation

struct sha1{
	int nbBlocs; //OK
	uint32_t* mot[16];
	uint32_t registre1[5]; //OK
	uint32_t registre2[5]; //OK
};typedef struct sha1 SHA1;

void myitoa(int in,char* out,int longueur){
	//QUITTE SI CEST >256
	int i,quotient,reste;
	int j=longueur;
	char g[2];
	int compteur=0;
	for(i=0;i<j-1;i++){
		out[i]='0';
	}
	out[j-1]='\0';
	while(in!=0){
		quotient=in/2;
		reste=in%2;
		if(reste==0) out[j-2-compteur]='0';
		else out[j-2-compteur]='1';
		compteur++;
		in=quotient;
	}
}

// F1 pour 0<=t<=19
uint32_t f1(uint32_t B,uint32_t C,uint32_t D){
	uint32_t f1=((B & C) | (!B & D));
	return f1;
}

// F2 pour 20<=t<=39
uint32_t f2(uint32_t B,uint32_t C,uint32_t D){
	uint32_t f2=B ^ C ^ D;
	return f2;
}

// F3 pour 40<=t<=59
uint32_t f3(uint32_t B,uint32_t C,uint32_t D){
	uint32_t f3=((B & C) | (B & D) | (C & D));
	return f3;
}

// F4 pour 60<=t<=79
uint32_t f4(uint32_t B,uint32_t C,uint32_t D){
	uint32_t f4=B ^ C ^ D;
	return f4;
}

int converbi(char* chaine){ //TAILLE 4
	int out=0;
	double rang=3;
	int caset=0;
	while(rang!=-1){
		if(chaine[caset]=='1') {out=out+pow(2,rang);}
		rang--;
		caset++;
	}
	return out;
}

uint32_t converhexa(char* chaine){ //TAILLE 9
	uint32_t out=0;
	double rang=8;
	int caset=0;
	while(rang!=-1){
		if(chaine[caset]=='1') {out+pow(16,rang);}
		else if(chaine[caset]=='2') {out+2*pow(16,rang);}
		else if(chaine[caset]=='3') {out+3*pow(16,rang);}
		else if(chaine[caset]=='4') {out+4*pow(16,rang);}
		else if(chaine[caset]=='5') {out+5*pow(16,rang);}
		else if(chaine[caset]=='6') {out+6*pow(16,rang);}
		else if(chaine[caset]=='7') {out+7*pow(16,rang);}
		else if(chaine[caset]=='8') {out+8*pow(16,rang);}
		else if(chaine[caset]=='9') {out+9*pow(16,rang);}
		else if(chaine[caset]=='a') {out+10*pow(16,rang);}
		else if(chaine[caset]=='b') {out+11*pow(16,rang);}
		else if(chaine[caset]=='c') {out+12*pow(16,rang);}
		else if(chaine[caset]=='d') {out+13*pow(16,rang);}
		else if(chaine[caset]=='e') {out+14*pow(16,rang);}
		else if(chaine[caset]=='f') {out+15*pow(16,rang);}
		rang--;
		caset++;
	}
	return out;
}

void remplit_zeros(char* t,int taille){
	int i;
	for(i=0;i<taille;i++){
		t[i]='0';
	}
	t[taille-1]='\0';
}

void remplit_valeur(char* t,int rang,int val){
	if(val==0) t[rang]='0';
	else if(val==1) t[rang]='1';
	else if(val==2) t[rang]='2';
	else if(val==3) t[rang]='3';
	else if(val==4) t[rang]='4';
	else if(val==5) t[rang]='5';
	else if(val==6) t[rang]='6';
	else if(val==7) t[rang]='7';
	else if(val==8) t[rang]='8';
	else if(val==9) t[rang]='9';
	else if(val==10) t[rang]='a';
	else if(val==11) t[rang]='b';
	else if(val==12) t[rang]='c';
	else if(val==13) t[rang]='d';
	else if(val==14) t[rang]='e';
	else if(val==15) t[rang]='f';
}

void affiche(char* message,int longueur){
	int i;
	for(i=0;i<longueur;i++){
		if((i%8==7)) printf("%c ",message[i]);
		else printf("%c",message[i]);
		if(i%32==31) printf("\n");
	}
	printf("\n");
}

SHA1 init_registres(SHA1 hash){
	hash.registre1[0]=0x67452301;
	hash.registre1[1]=0xEFCDAB89;
	hash.registre1[2]=0x98BADCFE;
	hash.registre1[3]=0x10325476;
	hash.registre1[4]=0xC3D2E1F0;
	
	hash.registre2[0]=hash.registre1[0];
	hash.registre2[1]=hash.registre1[1];
	hash.registre2[2]=hash.registre1[2];
	hash.registre2[3]=hash.registre1[3];
	hash.registre2[4]=hash.registre1[4];

	return hash;
}

SHA1 init_sha1(char* message){
	SHA1 hash;
	hash=init_registres(hash);
	int taille=strlen(message); //+1
	//QUITTE SI >2^64
	int nb=taille*sizeof(char)*TAILLE_OCTET+1+1;//POUR 1 EN PLUS
	//printf("->%d\n",nb);
	char binaire[nb]; //contiendra le binaire du message
	binaire[0]='\0';
	char lettre[sizeof(char)*TAILLE_OCTET+1]; //contiendra le binaire de la lettre
	char c; int i=0; int j;
	for(j=0;j<strlen(message);j++){
		c=message[j];
		//printf("%c : ",c);
		myitoa(c,lettre,sizeof(lettre));
		//printf("%s\n",lettre);
		strcat(binaire,lettre);
	}
	//printf("V1 : "); //V1: binaire du message OK
	//affiche(binaire,sizeof(binaire)); 
	char temp[2];
	temp[0]='1';temp[1]='\0';
	strcat(binaire,temp);

	//printf("V2 : "); //V2: binaire du message + 1 OK
	//affiche(binaire,sizeof(binaire));

	int calc=0;
	int k;
	for(k=0;k<512;k++){
		if((strlen(binaire)+64+k)%512==0){ 
			calc=k;
		}
	}
	hash.nbBlocs=(strlen(binaire)+64+calc)/512; //
	
	char blocZero[calc+1];
	remplit_zeros(blocZero,calc+1);
	char finalBinaire[strlen(binaire)+64+calc+1]; 
	finalBinaire[0]='\0';
	strcat(finalBinaire,binaire);
	strcat(finalBinaire,blocZero);
	//printf("V3 : "); //V3: binaire du message + 1 + n*'0' OK
	//affiche(finalBinaire,sizeof(finalBinaire)-64);
	
	char blocTaille[64+1];
	remplit_zeros(blocTaille,sizeof(blocTaille));
	myitoa(taille*TAILLE_OCTET,blocTaille,sizeof(blocTaille));
	strcat(finalBinaire,blocTaille);
	//printf("V4 : "); //V4: binaire du message + 1 + n*'0' + taille du message en binaire
	//affiche(finalBinaire,sizeof(finalBinaire));
		
	char finalHexa[strlen(finalBinaire)/4+1];
	finalHexa[sizeof(finalHexa)-1]='\0';
	int grQuatre;
	int p=0;int rang=0;
	char paquet[5]; paquet[4]='\0';
	do{
		paquet[0]=finalBinaire[p];
		paquet[1]=finalBinaire[p+1];
		paquet[2]=finalBinaire[p+2];
		paquet[3]=finalBinaire[p+3];
		grQuatre=converbi(paquet);
		remplit_valeur(finalHexa,rang,grQuatre);
		rang++;
		p=p+4;
	}while(p!=strlen(finalBinaire));
	printf("FINAL : \n");
	affiche(finalHexa,sizeof(finalHexa));
	//hash.mot=malloc(hash.nbBlocs * sizeof(uint32_t*));
	//PAS SUR
	int d;
	for(d=0;d<16;d++){
		hash.mot[d]=malloc(hash.nbBlocs * sizeof(uint32_t));
	}
	p=0;rang=0;
	uint32_t grHuit;
	char paquet1[9]; paquet1[8]='\0';
	do{
		paquet1[0]=finalHexa[p];
		//printf("%c",paquet1[0]);
		paquet1[1]=finalHexa[p+1];
		//printf("%c",paquet1[1]);
		paquet1[2]=finalHexa[p+2];
		//printf("%c",paquet1[2]);
		paquet1[3]=finalHexa[p+3];
		//printf("%c",paquet1[3]);		
		paquet1[4]=finalHexa[p+4];
		//printf("%c",paquet1[4]);
		paquet1[5]=finalHexa[p+5];
		//printf("%c",paquet1[5]);
		paquet1[6]=finalHexa[p+6];
		//printf("%c",paquet1[6]);
		paquet1[7]=finalHexa[p+7];
		//printf("%c",paquet1[7]);
		grHuit=converhexa(paquet1);
		//printf(">%u\n",grHuit);
		printf("%s\n",paquet1);
		//printf("\n");
		rang++;
		p=p+8;
	}while(p!=strlen(finalHexa));
	return hash;
}

void libere_memoire(SHA1 hash){
	int i;
	for(i=0;i<16;i++){
		free(hash.mot[i]);
	}
}


int main(){
	SHA1 hash;
	hash=init_sha1("clement");
	
	/*uint32_t i=0xAF;
	uint32_t ii=0x25;
	uint32_t iii=0xFF;
	printf("%u ", f2(i,ii,iii));*/
	libere_memoire(hash);
	return 0;
}
