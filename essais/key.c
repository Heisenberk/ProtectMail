#include <stdio.h>
#include <stdlib.h>
#include <tomcrypt.h>

int main(){
	/*rsa_key key;
	int err;
	register_prng(&sprng_desc);
	if ((err = rsa_make_key(NULL, find_prng("sprng"), 1024/8, 3, &key))!= CRYPT_OK) {
		printf("make_key error: %s\n", error_to_string(err));
		return -1;
		//printf("AA\n");
	}*/
	rsa_key key;
	int err;
	int prng_idx=find_prng("sprng");
	printf("%d\n",prng_idx);
	//register_prng(&sprng_desc);
	if ((err = rsa_make_key(NULL, prng_idx, 1024/8, 257, &key))!= CRYPT_OK) {
		printf("make_key error: %s\n", error_to_string(err));
		return -1;
		//printf("AA\n");
	}
	return 0;
}
