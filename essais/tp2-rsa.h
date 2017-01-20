/*
 * info-614,
 * entêtes pour le tp2
 */

#ifndef _TP2_RSA_H
#define _TP2_RSA_H

/* bibliothèques qui peuvent servir... */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <gmp.h>     /* grands entiers */

#include <getopt.h>  /* gestion des arguments optionels de la ligne de commandes */
#include <time.h>    /* pour initialiser la graine pour générer des nombres aléatoires */


#define MAX_SIZE 10000

/*
 * écriture d'une clé (privée ou publique) dans un fichier
 * Le format du fichier est simplement :
 *   - première ligne : base de la clé en base 62
 *   - seconde ligne : clé (d ou e) en base 62
 *
 * Attention, le fichier est écrasé s'il existait.
 */
void ecrire_cle(const char *fichier, const mpz_t base, const mpz_t cle_e) ;


/*
 * lecture d'une clé (base et clé) dans un fichier.
 * Le format du fichier est simplement :
 *   - première ligne : base de la clé en base 62
 *   - seconde ligne : clé en base 62
 */
void lire_cle(const char *fichier, mpz_t base, mpz_t cle_e) ;


/*
 * génère une paire de clés privée / publique.
 *   - "taille_base" est la taille, en bits, de la base (second argument)
 *   - base est la base (c'est le produit des nombres p et q générés
 *     temporairement dans la fonction)
 *   - cle_d et cle_e sont la clé privée et publique
 */
void genere_cles(int taille_base, mpz_t base, mpz_t cle_d, mpz_t cle_e) ;


/*
 * écrit "nb_octets" octets du nombre "bloc" dans le fichier. Si l'entier est trop petit,
 * complète par des 0 devant pour ecrire exactement "nb_octets" octets.
 * La valeur de retour est le nombre total d'octets écrits.
 */
int mpz_fwrite(const mpz_t bloc, int nb_octets, FILE *fichier) ;



/*
 * lit au plus "nb_octets" octets dans un fichier et les met dans un grand entier.
 * La valeur de retour est le nombre d'octets lus.
 */
int mpz_fread(mpz_t bloc, int nb_limbs, FILE *fichier) ;


/* codage d'un fichier */
int coder_RSA(const mpz_t base, const mpz_t cle_e, FILE *fichier_clair, FILE *fichier_crypte);

/* décodage d'un fichier */
int decoder_RSA(const mpz_t base, const mpz_t cle_d, FILE *fichier_crypte, FILE *fichier_clair);



#endif  /* _TP2_RSA_H */
