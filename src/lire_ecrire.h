/**
 * \file lire_ecrire.h
 * \author Claire Baskevitch - Clément Caumes
 * \date 2017
 * \brief contient les signatures des fonctions concernant les entrees/sorties
 */

#include <gmp.h>
#include "pgp.h"
#include "types.h"

#ifndef __LIRE_ECRIRE_H
#define __LIRE_ECRIRE_H

/**
 * \fn void affiche_date_heure()
 * \brief affiche la date et l'heure
 * */
void affiche_date_heure();

/**
 * \fn void affiche_general()
 * \brief affiche les informations generales de l'application
 * */
void affiche_general();

/**
 * \fn void affiche_commandes()
 * \brief affiche les commandes proposees par l'application
 * */
void affiche_commandes();

/**
 * \fn char* concatenation_extension(char* s)
 * \brief renvoie la concatenation de s avec ".pgp"
 * \param *s chaine de caracteres a modifier
 * \return retourne avec ".pgp" en plus a la fin
 * */
char *concatenation_extension(char *s);

/**
 * \fn int demande_taille_cles()
 * \brief retourne la taille des cles voulu par l'utilisateur
 * \return retourne la taille de la cle
 * */
int demande_taille_cles();

/**
 * \fn void cherche_cle_pub(mpz_t n, mpz_t e)
 * \brief demande a lutilisateur la cle et la stocke
 * \param n cle publique
 * \param e cle publique
 * */
void cherche_cle_pub(mpz_t n, mpz_t e);

/**
 * \fn void cherche_cle_priv(mpz_t n, mpz_t d)
 * \brief demande a lutilisateur la cle et la stocke
 * \param n cle privee
 * \param d cle privee
 * */
void cherche_cle_priv(mpz_t n, mpz_t d);

/**
 * \fn void ecrit_bordure_sup_rsa_pub(FILE* f)
 * \brief affichage fichier
 * \param *f fichier ou ecrire
 * */
void ecrit_bordure_sup_rsa_pub(FILE * f);

/**
 * \fn void ecrit_bordure_sup_rsa_priv(FILE* f)
 * \brief affichage fichier
 * \param *f fichier ou ecrire
 * */
void ecrit_bordure_sup_rsa_priv(FILE * f);

/**
 * \fn void ecrit_bordure_sup_m_sig(FILE* f)
 * \brief affichage fichier
 * \param *f fichier ou ecrire
 * */
void ecrit_bordure_sup_m_sig(FILE * f);

/**
 * \fn void ecrit_bordure_sup_m_chiffre(FILE* f)
 * \brief affichage fichier
 * \param *f fichier ou ecrire
 * */
void ecrit_bordure_sup_m_chiffre(FILE * f);

/**
 * \fn void ecrit_bordure_sup_id(FILE* f,char* s1,char* s2,char* s3)
 * \brief affichage fichier
 * \param *f fichier ou ecrire
 * \param *s1 nom
 * \param *s2 prenom
 * \param *s3 mail
 * */
void ecrit_bordure_sup_id(FILE * f, char *s1, char *s2, char *s3);

/**
 * \fn void ecrit_bordure_inf_m_sig(FILE* f)
 * \brief affichage fichier
 * \param *f fichier ou ecrire
 * */
void ecrit_bordure_inf_m_sig(FILE * f);

/**
 * \fn void ecrit_bordure_inf_m_chiffre(FILE* f)
 * \brief affichage fichier
 * \param *f fichier ou ecrire
 * */
void ecrit_bordure_inf_m_chiffre(FILE * f);

/**
 * \fn void ecrit_bordure_inf_rsa_pub(FILE* f)
 * \brief affichage fichier
 * \param *f fichier ou ecrire
 * */
void ecrit_bordure_inf_rsa_pub(FILE * f);

/**
 * \fn void ecrit_bordure_inf_rsa_priv(FILE* f)
 * \brief affichage fichier
 * \param *f fichier ou ecrire
 * */
void ecrit_bordure_inf_rsa_priv(FILE * f);

/**
 * \fn void ecrit_bordure_inf_m_sig_final(FILE* f)
 * \brief affichage fichier
 * \param *f fichier ou ecrire
 * */
void ecrit_bordure_inf_m_sig_final(FILE * f);

/**
 * \fn void affiche_action_pgp(char* nom)
 * \brief affichage terminal
 * \param *nom nom a afficher
 * */
void affiche_action_pgp(char *nom);

/**
 * \fn void affiche_dechiffrement(char* nomFichier)
 * \brief affichage terminal
 * \param *nomFichier nom a afficher
 * */
void affiche_dechiffrement(char *nomFichier);

/**
 * \fn void affiche_fichier_dechiffre(char* nomFichier)
 * \brief affichage terminal
 * \param *nomFichier nom a afficher
 * */
void affiche_fichier_dechiffre(char *nomFichier);

/**
 * \fn void recopie_message(FILE* origin,FILE* new)
 * \brief recopie origin dans new
 * \param *origin fichier
 * \param *new fichier
 * */
void recopie_message(FILE * origin, FILE * new);

/**
 * \fn void affiche_contenu_fic(char* nomFichier)
 * \brief affiche sur le terminal le fichier 
 * \param *nomFichier nom du fichier a afficher
 * */
void affiche_contenu_fic(char *nomFichier);

/**
 * \fn int teste_reponse(char* s)
 * \brief teste si l'utilisateur veut dire OUI ou NON
 * \param *s "chaine de caracteres pour determiner 
 * \return 1 pour oui et 2 pour non
 * */
int teste_reponse(char *s);

/**
 * \fn void demande_visualisation_message(char* message)
 * \brief procédure qui affiche le contenu d'un fichier si nécessaire
 * \param *message message a afficher
 * */
void demande_visualisation_message(char *message);

/**
 * \fn void ecrit_cle_privee(mpz_t n,mpz_t d)
 * \brief ecrit la clé privée dans le fichier secring.pgp
 * \param n parametre de la cle privee
 * \param d parametre de la cle privee
 * */
void ecrit_cle_privee(mpz_t n, mpz_t d);

/**
 * \fn void ecrit_cle_publique(char* s1,char* s2,char* s3,mpz_t n,mpz_t e)
 * \brief ecrit la clé publique dans le fichier pubring.pgp
 * \param *s1 nom
 * \param *s2 prenom
 * \param *s3 adresse mail
 * \param n parametre de la cle publique
 * \param e parametre de la cle publique
 * */
void ecrit_cle_publique(char *s1, char *s2, char *s3, mpz_t n, mpz_t e);

/**
 * \fn int compte_nb_car_fichier(FILE* f)
 * \brief compte le nombre de caracteres dans le fichier f
 * \param *f fichier 
 * \return entier correspondant au nombre de caracteres dans f
 * */
int compte_nb_car_fichier(FILE * f);

/**
 * \fn int compte_nb_car_fichier_signature(FILE* f)
 * \brief compte le nombre de caracteres dans le fichier.pgp avec signature f
 * \param *f fichier 
 * \return entier correspondant au nombre de caracteres dans f
 * */
int compte_nb_car_fichier_signature(FILE * f);

/**
 * \fn void remplit_chaine_carac_message(FILE* f,char* message,int taille)
 * \brief fonction qui met les caracteres du fichier dans message
 * \param *f fichier 
 * \param *message chaine de caracteres
 * \param taille taille de la chaine de caracteres
 * */
void remplit_chaine_carac_message(FILE * f, char *message, int taille);

/**
 * \fn void remplit_chaine_carac_message_signe(FILE* f,char* message,int taille)
 * \brief fonction qui met les caracteres du fichier.pgp avec signature dans message
 * \param *f fichier 
 * \param *message chaine de caracteres
 * \param taille taille de la chaine de caracteres
 * */
void remplit_chaine_carac_message_signe(FILE * f, char *message, int taille);

/**
 * \fn int teste_fichier_signature(FILE* f)
 * \brief fonction qui teste si le fichier f est bien un fichier.pgp avec signature 
 * \param *f fichier 
 * \return 1 si il s'agit bien d'un .pgp sinon 0
 * */
int teste_fichier_signature(FILE * f);

#endif
