# Claire Baskevitch - Clément Caumes
# Année 2017
# Projet Pretty Good Privacy (PGP)

# Makefile du projet PGP

CFLAGS=-c -g -Wall
SRC=src
BLD=build

compil: pgp.o envoi.o reception.o commandes.o lire_ecrire.o gestion_cles.o math_crypto.o build
	gcc -Wall -o pgp $(BLD)/pgp.o $(BLD)/envoi.o $(BLD)/reception.o $(BLD)/commandes.o $(BLD)/math_crypto.o $(BLD)/lire_ecrire.o $(BLD)/gestion_cles.o -lgmp -lm

pgp.o: $(SRC)/pgp.c $(SRC)/pgp.h $(SRC)/envoi.h $(SRC)/reception.h $(SRC)/commandes.h $(SRC)/lire_ecrire.h $(SRC)/gestion_cles.h $(SRC)/math_crypto.h $(SRC)/types.h build
	gcc $(CFLAGS) $(SRC)/pgp.c -o $(BLD)/pgp.o

envoi.o: $(SRC)/envoi.c $(SRC)/pgp.h $(SRC)/envoi.h $(SRC)/commandes.h $(SRC)/lire_ecrire.h $(SRC)/gestion_cles.h $(SRC)/math_crypto.h $(SRC)/types.h build
	gcc $(CFLAGS) $(SRC)/envoi.c -o $(BLD)/envoi.o
	
reception.o: $(SRC)/reception.c $(SRC)/pgp.h $(SRC)/reception.h $(SRC)/gestion_cles.h $(SRC)/lire_ecrire.h $(SRC)/math_crypto.h $(SRC)/types.h build
	gcc $(CFLAGS) $(SRC)/reception.c -o $(BLD)/reception.o
	
commandes.o: $(SRC)/commandes.c $(SRC)/pgp.h $(SRC)/commandes.h $(SRC)/lire_ecrire.h $(SRC)/math_crypto.h $(SRC)/types.h build
	gcc $(CFLAGS) $(SRC)/commandes.c -o $(BLD)/commandes.o
	
lire_ecrire.o: $(SRC)/lire_ecrire.c $(SRC)/pgp.h $(SRC)/lire_ecrire.h $(SRC)/commandes.h $(SRC)/gestion_cles.h $(SRC)/math_crypto.h $(SRC)/types.h build
	gcc $(CFLAGS) $(SRC)/lire_ecrire.c -o $(BLD)/lire_ecrire.o
	
gestion_cles.o: $(SRC)/gestion_cles.c $(SRC)/pgp.h $(SRC)/gestion_cles.h $(SRC)/lire_ecrire.h $(SRC)/commandes.h $(SRC)/math_crypto.h $(SRC)/types.h build
	gcc $(CFLAGS) $(SRC)/gestion_cles.c -o $(BLD)/gestion_cles.o
	
math_crypto.o: $(SRC)/math_crypto.c $(SRC)/math_crypto.h $(SRC)/pgp.h $(SRC)/gestion_cles.h $(SRC)/lire_ecrire.h $(SRC)/commandes.h $(SRC)/types.h build
	gcc $(CFLAGS) $(SRC)/math_crypto.c -o $(BLD)/math_crypto.o
	
build:
	mkdir -p build/
	
	
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
