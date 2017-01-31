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
	uint32_t* mot[16]; //OK
	uint32_t registre1[5]; //OK //A,B,C,D,E
	uint32_t registre2[5]; //OK //H0,H1,H2,H3,H4,H5
	uint32_t W[80];
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

uint32_t f(uint32_t B,uint32_t C,uint32_t D,int t){
	uint32_t f;
	if((0<=t)&&(t<=19)) f=((B & C) | ((~B) & D)); //F1
	else if((20<=t)&&(t<=39)) f=B ^ C ^ D; //F2
	else if((40<=t)&&(t<=59)) f=((B & C) | (B & D) | (C & D)); //F3
	else if((60<=t)&&(t<=79)) f=B ^ C ^ D; //F4
	else f=-1;
	return f;
}

uint32_t k(int t){
	uint32_t out;
	if((0<=t)&&(t<=19)) out=K1;
	else if((20<=t)&&(t<=39)) out=K2;
	else if((40<=t)&&(t<=59)) out=K3;
	else if((60<=t)&&(t<=79)) out=K4;
	else out=-1;
	return out;
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
	uint32_t puiss;
	double rang=8-1;
	int caset=0;
	//printf(">>>%s\n",chaine);
	while(rang!=-1){
		//printf("!%f et %c\n",rang,chaine[caset]);
		//printf("%f\n",pow(16,rang));
		puiss=pow(16,rang);
		//printf("16^%f->%u*%c",rang,puiss,chaine[caset]);
		//printf("!%u\n",out);
		if(chaine[caset]=='1') {out=out+puiss;}
		else if(chaine[caset]=='2') {out=out+2*puiss;}
		else if(chaine[caset]=='3') {out=out+3*puiss;}
		else if(chaine[caset]=='4') {out=out+4*puiss;}
		else if(chaine[caset]=='5') {out=out+5*puiss;}
		else if(chaine[caset]=='6') {out=out+6*puiss;}
		else if(chaine[caset]=='7') {out=out+7*puiss;}
		else if(chaine[caset]=='8') {out=out+8*puiss;}
		else if(chaine[caset]=='9') {out=out+9*puiss;}
		else if(chaine[caset]=='a') {out=out+10*puiss;}
		else if(chaine[caset]=='b') {out=out+11*puiss;}
		else if(chaine[caset]=='c') {out=out+12*puiss;}
		else if(chaine[caset]=='d') {out=out+13*puiss;}
		else if(chaine[caset]=='e') {out=out+14*puiss;}
		else if(chaine[caset]=='f') {out=out+15*puiss;}
		//printf(" = %u\n",out);
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
	hash.registre2[0]=0x67452301;
	hash.registre2[1]=0xEFCDAB89;
	hash.registre2[2]=0x98BADCFE;
	hash.registre2[3]=0x10325476;
	hash.registre2[4]=0xC3D2E1F0;
	//H0,H1,H2,H3,H4,H5 initialises
	
	//A ENLEVER
	hash.registre1[0]=hash.registre2[0];
	hash.registre1[1]=hash.registre2[1];
	hash.registre1[2]=hash.registre2[2];
	hash.registre1[3]=hash.registre2[3];
	hash.registre1[4]=hash.registre2[4];
	//printf("ICI:%0x %0x %0x %0x %0x\n",hash.registre2[0],hash.registre2[1],hash.registre2[2],hash.registre2[3],hash.registre2[4]);
	return hash;
}

SHA1 init_16W(SHA1 hash,int rang){
	int i;
	for(i=0;i<16;i++){
		hash.W[i]=hash.mot[i][rang];
	}
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
	//printf("!!!%d\n",strlen(finalBinaire));
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
		paquet1[1]=finalHexa[p+1];
		paquet1[2]=finalHexa[p+2];
		paquet1[3]=finalHexa[p+3];		
		paquet1[4]=finalHexa[p+4];
		paquet1[5]=finalHexa[p+5];
		paquet1[6]=finalHexa[p+6];
		paquet1[7]=finalHexa[p+7];
		grHuit=converhexa(paquet1);
		printf(">hash.mot[%d][%d]=%0x\n",(p/(8))%16,(p/(16*8)),grHuit);
		hash.mot[(p/8)%16][p/(16*8)]=grHuit;
		rang++;
		p=p+8;
	}while(p!=strlen(finalHexa));
	return hash;
}

void process_sha1(SHA1 hash){
	int i,t;
	uint32_t TEMP;
	for(i=0;i<hash.nbBlocs;i++){
		hash=init_16W(hash,i);
		printf("H     :%0x %0x %0x %0x %0x\n",hash.registre2[0],hash.registre2[1],hash.registre2[2],hash.registre2[3],hash.registre2[4]);
		printf("LETTRE:%0x %0x %0x %0x %0x\n",hash.registre1[0],hash.registre1[1],hash.registre1[2],hash.registre1[3],hash.registre1[4]);
		printf("OK ICI\n\n");
		//AFFICHAGE
		int h;
		for(h=0;h<16;h++){
			printf("W%d:%0x\n",h,hash.W[h]);
		}
		////
		for(t=16;t<80;t++){
			hash.W[t]=((hash.W[t-3] ^ hash.W[t-8] ^ hash.W[t-14] ^ hash.W[t-16])<<1);
			printf("W%d:%0x\n",t,hash.W[t]);
		}
		hash.registre1[0]=hash.registre2[0]; //A<-HO
		hash.registre1[1]=hash.registre2[1]; //B<-H1
		hash.registre1[2]=hash.registre2[2]; //C<-H2
		hash.registre1[3]=hash.registre2[3]; //D<-H3
		hash.registre1[4]=hash.registre2[4]; //E<-H4
		printf("H     :%0x %0x %0x %0x %0x\n",hash.registre2[0],hash.registre2[1],hash.registre2[2],hash.registre2[3],hash.registre2[4]);
		printf("LETTRE:%0x %0x %0x %0x %0x\n",hash.registre1[0],hash.registre1[1],hash.registre1[2],hash.registre1[3],hash.registre1[4]);
		printf("OK ICI\n\n");


		for(t=0;t<80;t++){
			TEMP=(hash.registre1[0]<<5)+f(hash.registre1[1],hash.registre1[2],hash.registre1[3],t)+hash.registre1[4]+hash.W[t]+k(t);
			printf("TEMP:%0x\n",TEMP);
			hash.registre1[4]=hash.registre1[3];
			hash.registre1[3]=hash.registre1[2];
			hash.registre1[2]=hash.registre1[1]<<30;//36
			hash.registre1[1]=hash.registre1[0];
			hash.registre1[0]=TEMP;
			printf(">>%0x %0x %0x %0x %0x\n",hash.registre1[0],hash.registre1[1],hash.registre1[2],hash.registre2[3],hash.registre1[4]);
		}
		printf("BBB:%0x %0x %0x %0x %0x\n",hash.registre1[0],hash.registre1[1],hash.registre1[2],hash.registre1[3],hash.registre1[4]);

		printf("->%0x %0x %0x %0x %0x\n",hash.registre2[0],hash.registre2[1],hash.registre2[2],hash.registre2[3],hash.registre2[4]);
		hash.registre2[0]=hash.registre2[0]+hash.registre1[0];
		hash.registre2[1]=hash.registre2[1]+hash.registre1[1];
		hash.registre2[2]=hash.registre2[2]+hash.registre1[2];
		hash.registre2[3]=hash.registre2[3]+hash.registre1[3];
		hash.registre2[4]=hash.registre2[4]+hash.registre1[4];
	}
	printf("->%0x %0x %0x %0x %0x\n",hash.registre2[0],hash.registre2[1],hash.registre2[2],hash.registre2[3],hash.registre2[4]);
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
	process_sha1(hash);
	//printf("%d\n",hash.nbBlocs);
	libere_memoire(hash);
	return 0;
}
