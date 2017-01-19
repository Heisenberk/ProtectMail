#include "pgp.h"
#include "types.h"

#ifndef __RECEPTION_H
#define __RECEPTION_H

int teste_egalite_hashes(unsigned char* hash1, unsigned char* hash2);
void verifie_authentification(char* nomFichier);
void cree_fichier_dechiffre(char* nomFichier);

#endif
