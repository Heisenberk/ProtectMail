#include <inttypes.h>
#define NB_CAR_UINT64 20

/* Valeur maximal d'une variable de 64 bits */
#define MAX_UINT64 0xFFFFFFFFFFFFFFFF

#define max(a,b) ((a<b)?(b):(a))
#define min(a,b) ((a<b)?(a):(b))

/** Type uint_x
 * taille est le nombre de blocs de 64 bits
 * tab est un pointeur sur un tableau de type uint64 */
typedef struct uint_x{
	int taille;
	uint64_t* tab;
} UINT_X;

int nb_bits_uint64(uint64_t a);
char access_bit_n(uint64_t a,int n);
int nb_bits_uint_x(UINT_X a);

UINT_X malloc_uint_x(int taille);
void init_zero(UINT_X *n);
void free_uint_x(UINT_X n);
int somme (UINT_X *resultat, UINT_X a, UINT_X b);
void somme1(UINT_X *resultat, UINT_X a, UINT_X b);

void printf_binaire_uint_x(UINT_X n);

