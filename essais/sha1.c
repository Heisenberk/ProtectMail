#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#define TAILLE_OCTET 8

// -lm compilation

void myitoa(int in,char* out,int longueur){
	//QUITTE SI CEST >256
	int i,quotient,reste;
	//int j=sizeof(out)/sizeof(char);
	int j=longueur;
	char g[2];
	//printf("MYITOA:%lu\n",sizeof(out));
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
	//printf("%s\n",out);
}

//ATTEN
int converbi(char* chaine){
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
	else if(val==10) t[rang]='A';
	else if(val==11) t[rang]='B';
	else if(val==12) t[rang]='C';
	else if(val==13) t[rang]='D';
	else if(val==14) t[rang]='E';
	else if(val==15) t[rang]='F';
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

void sha1(char* message){
	int taille=strlen(message); //+1
	//QUITTE SI >64
	int nb=taille*sizeof(char)*TAILLE_OCTET+1+1;//POUR 1 EN PLUS
	//printf("->%d\n",nb);
	char binaire[nb]; //contiendra le binaire du message
	binaire[0]='\0';
	char lettre[sizeof(char)*TAILLE_OCTET+1]; //contiendra le binaire de la lettre
	char c; int i=0; int j;
	for(j=0;j<strlen(message);j++){
		c=message[j];
		printf("%c : ",c);
		myitoa(c,lettre,sizeof(lettre));
		printf("%s\n",lettre);
		strcat(binaire,lettre);
	}
	printf("V1 : "); //V1: binaire du message OK
	affiche(binaire,sizeof(binaire)); 
	char temp[2];
	temp[0]='1';temp[1]='\0';
	strcat(binaire,temp);

	printf("V2 : "); //V2: binaire du message + 1 OK
	affiche(binaire,sizeof(binaire));

	int calc=0;
	int k;
	for(k=0;k<512;k++){
		if((strlen(binaire)+64+k)%512==0){ 
			calc=k;
		}
	}
	char blocZero[calc+1];
	remplit_zeros(blocZero,calc+1);
	char finalBinaire[strlen(binaire)+64+calc+1]; 
	finalBinaire[0]='\0';
	strcat(finalBinaire,binaire);
	strcat(finalBinaire,blocZero);
	printf("V3 : "); //V3: binaire du message + 1 + n*'0' OK
	affiche(finalBinaire,sizeof(finalBinaire)-64);
	
	char blocTaille[64+1];
	remplit_zeros(blocTaille,sizeof(blocTaille));
	myitoa(taille*TAILLE_OCTET,blocTaille,sizeof(blocTaille));
	strcat(finalBinaire,blocTaille);
	printf("V4 : "); //V4: binaire du message + 1 + n*'0' + taille du message en binaire
	affiche(finalBinaire,sizeof(finalBinaire));
		
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
	
}


int main(){
	sha1("clement");
	/*char binaire[5];
	int k,i;
	for(k=0;k<16;k++){
		i=k;
		myitoa(i,binaire,sizeof(binaire));
		printf("%d,%s,%d\n",i,binaire,converbi(binaire));
	}*/
	return 0;
}
