/**
 * \file types.h
 * \author Claire Baskevitch - Clément Caumes
 * \date 2017
 * \brief contient les constantes utilisées dans le projet
 */

#ifndef __TYPES_H
#define __TYPES_H

/** MODES: différentes utilisations de l'application **/
#define MODE_SIGN_NN_CHIFFRE 1
#define MODE_DECHIFFREMENT 2
#define MODE_AFFICHAGE_DECHIFFRE 3
#define MODE_GENERATION_CLES 4
#define MODE_CHIFFREMENT 6
#define MODE_SIGN_CHIFFRE 7

/** Tailles utilisées dans le projet **/
#define TAILLE_CLE_SESSION 16
#define TAILLE 20
#define TAILLE_HASH_SHA1 (40)
#define TAILLE_OCTET (8)

/** Constantes utilisees pour le hash SHA-1 **/
#define K1 (0x5A827999)
#define K2 (0x6ED9EBA1)
#define K3 (0x8F1BBCDC)
#define K4 (0xCA62C1D6)

#endif
