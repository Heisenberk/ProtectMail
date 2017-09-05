//VERIFIE

#include <gmp.h>
#include "pgp.h"
#include "types.h"

#ifndef __LIRE_ECRIRE_H
#define __LIRE_ECRIRE_H

/*void affiche_general();
void affiche_commandes();
void affiche_action_pgp(char* nom);
int demande_taille_cles();

void ecrit_bordure_sup_rsa_priv(FILE* f);
void ecrit_bordure_sup_rsa_pub(FILE* f);
void ecrit_bordure_sup_m_sig(FILE* f);
void ecrit_bordure_sup_m_chiffre(FILE* f);

void ecrit_bordure_inf_m_sig(FILE* f);
void ecrit_bordure_inf_m_chiffre(FILE* f);
void ecrit_bordure_inf_rsa_priv(FILE* f);
void ecrit_bordure_inf_rsa_pub(FILE* f);
void recopie_message(FILE* origin,FILE* new);

void affiche_contenu_fic(char* nomFichier);
void affiche_fichier_dechiffre(char* nomFichier);
void demande_visualisation_message(char* nomFichier);

void cherche_cle_pub(mpz_t n, mpz_t e);
void cherche_cle_priv(mpz_t n, mpz_t d);

void ecrit_cle_privee(mpz_t n,mpz_t d);
void ecrit_cle_publique(char* s1,char* s2,char* s3,mpz_t n,mpz_t e);

void transfert_fic1_fic2(char* s1,char* s2);*/

void affiche_date_heure();
void affiche_general();
void affiche_commandes();

int demande_taille_cles();
void cherche_cle_pub(mpz_t n, mpz_t e);
void cherche_cle_priv(mpz_t n, mpz_t d);

void affiche_action_pgp(char* nom);

void ecrit_bordure_sup_rsa_pub(FILE* f);
void ecrit_bordure_sup_rsa_priv(FILE* f);
void ecrit_bordure_sup_m_sig(FILE* f);
void ecrit_bordure_sup_m_chiffre(FILE* f);
void ecrit_bordure_sup_id(FILE* f,char* s1,char* s2,char* s3);
void ecrit_bordure_inf_m_sig(FILE* f);
void ecrit_bordure_inf_m_chiffre(FILE* f);
void ecrit_bordure_inf_rsa_pub(FILE* f);
void ecrit_bordure_inf_rsa_priv(FILE* f);

void affiche_dechiffrement(char* nomFichier);
void affiche_fichier_dechiffre(char* nomFichier);
void recopie_message(FILE* origin,FILE* new);
void affiche_contenu_fic(char* nomFichier);
int teste_reponse(char* s);
void demande_visualisation_message(char* nomFichier);

void ecrit_cle_privee(mpz_t n,mpz_t d);
void ecrit_cle_publique(char* s1,char* s2,char* s3,mpz_t n,mpz_t e);

#endif
