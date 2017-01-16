#include <stdio.h>
#include <gcrypt.h>

int main(){
	if (!gcry_check_version (GCRYPT_VERSION))
	{
		fputs ("libgcrypt version mismatch\n", stderr);
		exit (2);
	}
	gcry_control (GCRYCTL_DISABLE_SECMEM, 0);
	gcry_control (GCRYCTL_INITIALIZATION_FINISHED, 0);
	return 0;
}
