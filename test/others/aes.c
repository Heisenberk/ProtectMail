#include <tomcrypt.h>


int main(void)
{
	/*unsigned char key[16], nonce[12], pt[32], ct[32], tag[16], tagcp[16];
	unsigned long taglen;
	int err;
	register_cipher(&aes_desc);
	taglen = sizeof(tag);
	if ((err =ccm_memory(find_cipher("aes"),key, 16,NULL,nonce, 12,NULL, 0,pt, 32,ct,tag, &taglen,CCM_ENCRYPT)) != CRYPT_OK) {
		printf("ccm_memory error %s\n", error_to_string(err));
		return -1;
	}
	
	int i;
	for(i=0;i<16;i++){
		printf("%u \n",tag[i]);
	}
	//printf("%lu\n",taglen);
	

	taglen = sizeof(tagcp);
	if ((err =ccm_memory(find_cipher("aes"),key, 16,NULL,nonce, 12,NULL, 0,ct, 32,pt,tagcp, &taglen,CCM_DECRYPT)) != CRYPT_OK) {
	printf("ccm_memory error %s\n", error_to_string(err));
	return -1;
	}*/

	unsigned char pt[301], ct[302], key[16];
	int i;
	/*for(i=0;i<8;i++){
		printf("%u \n",key[i]);
	}*/
	symmetric_key skey;
	int err;
	if ((err = aes_setup(key,16,0,&skey)) != CRYPT_OK) {
		printf("Setup error: %s\n",error_to_string(err));
		return -1;
	}
	aes_ecb_encrypt(pt,ct,&skey);
	for(i=0;i<8;i++){
		printf("%u \n",pt[i]);
	}
}
