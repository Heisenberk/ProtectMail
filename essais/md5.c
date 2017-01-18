#include <tomcrypt.h>
#include <stdio.h>
#include <stdlib.h>
//#include <string.h>

int main(){
	hash_state md;
	unsigned char* in="bakcms";
	unsigned char out[16];
	md5_init(&md);
	md5_process(&md,in,strlen(in));
	md5_done(&md,out);
	int i;
	printf("%s : ",in);
	for(i=0;i<16;i++){ //TJRS 16 
		//printf("%u ",out[i]);
		printf("%x",out[i]);
	}
	printf("\n");
	return 0;
}
