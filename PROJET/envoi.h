/**
 * \file envoi.h
 * \author Claire Baskevitch - Clément Caumes
 * \date 2017
 * \brief contient les signatures des fonctions concernant l'envoi de messages
 */

#include "pgp.h"
#include "types.h"
#include "gestion_cles.h"
#include <gmp.h>

#ifndef __ENVOI_H
#define __ENVOI_H

/**
 * \fn void ecrit_message_non_chiffre_signe(int num,char**chaineCarac)
 * \brief fonction qui ecrit un nouveau message avec sa signature 
 * \param num equivalent a argc
 * \param chaineCarac equivalent a argv
 * */
void ecrit_message_non_chiffre_signe(int num, char **chaineCarac);

/**
 * \fn void cree_fichier_chiffre(char* nomFichier)
 * \brief fonction qui ecrit un nouveau message chiffre (XOR avec une cle de session) et chiffrement RSA de cette cle
 * \param *nomFichier nom du fichier a chiffrer
 * */
void cree_fichier_chiffre(char *nomFichier);

#endif
