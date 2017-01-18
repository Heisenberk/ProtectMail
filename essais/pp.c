#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tomcrypt.h>
#define PASSPHRASE 3eeac98df7e7a8ab1c0ebb0f9dfc1
#define TAILLE_HASH_MD5 16

	/*hash_state md;
	unsigned char* in="bakcms";
	unsigned char out[16];
	md5_init(&md);
	md5_process(&md,in,strlen(in));
	md5_done(&md,out);*/
	
void md5(unsigned char* in,unsigned char* out){
	hash_state md;
	//unsigned char out[16];
	md5_init(&md);
	md5_process(&md,in,strlen(in));
	md5_done(&md,out);
}

void demande_pass_phrase(){
	printf("\033[01mEntrez la Pass Phrase: \033[0m\033[30m");
	char buffer1[256],buffer2[256];
	scanf("%s",buffer1);
	printf("\033[0m");
	printf("\033[01mEntrez de nouveau la Pass Phrase: \033[0m\033[30m");
	scanf("%s",buffer2);
	printf("\033[0m");
	if(!strcmp(buffer1,buffer2)){
		printf("EGAL\n");
		unsigned char md[TAILLE_HASH_MD5];
		unsigned char* mdp=(unsigned char*)buffer1;
		md5(mdp,md);
		int i;
		for(i=0;i<TAILLE_HASH_MD5;i++){ 
			printf("%x",md[i]);
		}
		printf("\n");
	}
	else printf("PAS EGAL\n");
}

void demande_pass_phrase2(unsigned char* hash){
	printf("\033[01mEntrez la Pass Phrase: \033[0m\033[30m");
	char buffer1[256],buffer2[256];
	scanf("%s",buffer1);
	printf("\033[0m");
	printf("\033[01mEntrez de nouveau la Pass Phrase: \033[0m\033[30m");
	scanf("%s",buffer2);
	printf("\033[0m");
	if(!strcmp(buffer1,buffer2)){
		unsigned char* mdp=(unsigned char*)buffer1;
		md5(mdp,hash);
		int i;
	for(i=0;i<TAILLE_HASH_MD5;i++){ 
		printf("%x",hash[i]);
	}
	printf("\n");
	}
	else exit(1);;
	
}
	

int main(){
	unsigned char hash[16];
	demande_pass_phrase2(hash);
	int i;
	for(i=0;i<TAILLE_HASH_MD5;i++){ 
		printf("%x",hash[i]);
	}
	printf("\n");
	
	return 0;
}
