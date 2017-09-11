# Claire Baskevitch - Clément Caumes
# Année 2017
# Projet Pretty Good Privacy (PGP)

# Makefile du projet PGP

CFLAGS=-c -g -Wall

compil: pgp.o envoi.o reception.o commandes.o lire_ecrire.o gestion_cles.o math_crypto.o
	gcc -Wall -o pgp pgp.o envoi.o reception.o commandes.o math_crypto.o lire_ecrire.o gestion_cles.o -lgmp -lm

pgp.o: pgp.c pgp.h envoi.h reception.h commandes.h lire_ecrire.h gestion_cles.h math_crypto.h types.h
	gcc $(CFLAGS) pgp.c

envoi.o: envoi.c pgp.h envoi.h commandes.h lire_ecrire.h gestion_cles.h math_crypto.h types.h
	gcc $(CFLAGS) envoi.c 
	
reception.o: reception.c pgp.h reception.h gestion_cles.h lire_ecrire.h math_crypto.h types.h
	gcc $(CFLAGS) reception.c
	
commandes.o: commandes.c pgp.h commandes.h lire_ecrire.h math_crypto.h types.h
	gcc $(CFLAGS) commandes.c
	
lire_ecrire.o: lire_ecrire.c pgp.h lire_ecrire.h commandes.h gestion_cles.h math_crypto.h types.h
	gcc $(CFLAGS) lire_ecrire.c
	
gestion_cles.o: gestion_cles.c pgp.h gestion_cles.h lire_ecrire.h commandes.h math_crypto.h types.h
	gcc $(CFLAGS) gestion_cles.c
	
math_crypto.o: math_crypto.c math_crypto.h pgp.h gestion_cles.h lire_ecrire.h commandes.h types.h
	gcc $(CFLAGS) math_crypto.c
	
	
indent: 
	indent -linux *.h
	indent -linux *.c
	rm -f *.h~
	rm -f *.c~
	
doxygen:
	doxygen *.h
	doxygen *.c
	
clean: 
	rm -f pgp
	rm -f *.o
