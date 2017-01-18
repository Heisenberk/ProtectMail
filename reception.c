#include "pgp.h"
#include "reception.h"
#include "types.h"

int teste_egalite_hashes(unsigned char* hash1, unsigned char* hash2){
	int i;
	for(i=0;i<TAILLE_HASH_MD5;i++){
		if(hash1[i]!=hash2[i]) return 0;
		i++;
	}
	return 1;
}
