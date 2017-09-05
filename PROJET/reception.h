//VERIFIE

#include "pgp.h"
#include "types.h"
#include "gestion_cles.h"

#ifndef __RECEPTION_H
#define __RECEPTION_H

/*int teste_egalite_hashes(unsigned char* hash1, unsigned char* hash2);
void verifie_authentification(char* nomFichier);
void cree_fichier_dechiffre(char* nomFichier);
void ecrit_message_dechiffre(char* nomFichier);*/

int teste_egalite_hashes(unsigned char* hash1, unsigned char* hash2);
void calcul_hash_message(char* nomFichier,unsigned char* hash_message);
void calcul_hash_signature(char* nomFichier,unsigned char* hash_signature);
int teste_signature(char* nomFichier);
void verifie_authentification(char* nomFichier);
void cree_fichier_dechiffre(char* nomFichier);
void ecrit_message_dechiffre(char* nomFichier);

#endif
