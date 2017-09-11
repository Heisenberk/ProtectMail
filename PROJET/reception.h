/**
 * \file reception.h
 * \author Claire Baskevitch - Clément Caumes
 * \date 2017
 * \brief contient les signatures des fonctions concernant la reception de messages
 */

#include "pgp.h"
#include "types.h"
#include "gestion_cles.h"

#ifndef __RECEPTION_H
#define __RECEPTION_H

/**
 * \fn int teste_egalite_hashes(char* hash1, char* hash2)
 * \brief fonction qui teste si hash1 et hash2 sont egaux
 * \param *hash1 chaine de caracteres 1
 * \param *hash2 chaine de caracteres 2
 * \return renvoie 1 si hash1 et hash2 sont égaux sinon 0
 * */
int teste_egalite_hashes(char *hash1, char *hash2);

/**
 * \fn void verifie_authentification(char* nomFichier)
 * \brief fonction qui verifie si le fichier a bien ete recu sans modification
 * \param *nomFichier nom du fichier a authentifier
 * */
void verifie_authentification(char *nomFichier);

/**
 * \fn void cree_fichier_dechiffre(char* nomFichier)
 * \brief fonction qui cree le fichier dechiffre
 * \param *nomFichier nom du fichier a dechiffrer
 * */
void cree_fichier_dechiffre(char *nomFichier);

/**
 * \fn void ecrit_message_dechiffre(char* nomFichier)
 * \brief fonction qui affiche sur le terminal le fichier dechiffre
 * \param *nomFichier nom du fichier a dechiffrer
 * */
void ecrit_message_dechiffre(char *nomFichier);

#endif
