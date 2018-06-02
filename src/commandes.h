/**
 * \file commandes.h
 * \author Claire Baskevitch - Clément Caumes
 * \date 2017
 * \brief contient la signature des fonctions concernant les commandes sur le terminal pour accéder aux fonctions de l'application
 */

#include "pgp.h"
#include "types.h"

#ifndef __COMMANDES_H
#define __COMMANDES_H

/**
 * \fn void quitte_commande_introuvable()
 * \brief fonction qui fait quitter l'application si la commande n'existe pas
 * */
void quitte_commande_introuvable();

/**
 * \fn void quitte_demande_invalide()
 * \brief fonction qui fait quitter l'application si la demande de l'utilisateur n'existe pas
 * */
void quitte_demande_invalide();

/**
 * \fn void quitte_fichier_inexistant()
 * \brief fonction qui fait quitter l'application si le fichier est inexistant
 * */
void quitte_fichier_inexistant();

/**
 * \fn void quitte_fichier_invalide()
 * \brief fonction qui fait quitter l'application si le fichier n'est pas valide (NULL)
 * */
void quitte_fichier_invalide();

/**
 * \fn void quitte_mauvaise_extension(char* s)
 * \brief fonction qui fait quitter l'application si le fichier est inexistant
 * \param s nom du fichier qui a la mauvaise extension
 * */
void quitte_mauvaise_extension(char *s);

/**
 * \fn void quitte_pass_phrase_incoherente()
 * \brief fonction qui fait quitter l'application si la pass phrase ecrite est différente du premier essai
 * */
void quitte_pass_phrase_incoherente();

/**
 * \fn void quitte_suppression_impossible(char* s)
 * \brief fonction qui fait quitter l'application si la pass phrase ecrite est différente du premier essai
 * \param s nom du fichier impossible a supprimer
 * */
void quitte_suppression_impossible(char *s);

/**
 * \fn void quitte_pas_probleme()
 * \brief fonction qui fait quitter l'application
 * */
void quitte_pas_probleme();

/**
 * \fn int teste_mots_identiques(char* s1,char* s2)
 * \brief fonction qui teste si deux chaines de caracteres sont identiques
 * \param s1 et s2 sont les deux chaines de caracteres a tester
 * \return 0 si les chaines de caracteres ne sont pas identiques
 * \return 1 si les chaines de caracteres sont identiques
 * */
int teste_mots_identiques(char *s1, char *s2);

/**
 * \fn int teste_extension_pgp(char* s)
 * \brief fonction qui teste si la chaine de caracteres finit par ".pgp"
 * \param s est la chaine de caracteres a tester
 * \return 0 si la chaine de caracteres ne finit pas par ".pgp"
 * \return 1 si la chaine de caracteres finit par ".pgp"
 * */
int teste_extension_pgp(char *s);

/**
 * \fn int teste_commande_une_option(int num,char** chaineCarac)
 * \brief fonction qui renvoie le mode choisi par l'utilisateur
 * \param num correspond a argc
 * \param chaineCarac correspond a argv
 * \return MODE_GENERATION_CLES ou MODE_DECHIFFREMENT
 * */
int teste_commande_une_option(int num, char **chaineCarac);

/**
 * \fn int teste_commande_deux_options(int num,char** chaineCarac)
 * \brief fonction qui renvoie le mode choisi par l'utilisateur
 * \param num correspond a argc
 * \param chaineCarac correspond a argv
 * \return MODE_SIGN_NN_CHIFFRE, MODE_AFFICHAGE_DECHIFFRE, MODE_CHIFFREMENT ou MODE_SIGN_CHIFFRE
 * */
int teste_commande_deux_options(int num, char **chaineCarac);

/**
 * \fn int teste_commande_general(int num,char** chaineCarac)
 * \brief fonction qui renvoie le mode choisi par l'utilisateur
 * \param num correspond a argc
 * \param chaineCarac correspond a argv
 * \return MODE_GENERATION_CLES, MODE_DECHIFFREMENT, MODE_SIGN_NN_CHIFFRE, MODE_AFFICHAGE_DECHIFFRE, MODE_CHIFFREMENT ou MODE_SIGN_CHIFFRE
 * */
int teste_commande_general(int num, char **chaineCarac);

#endif
