CFLAGS=-c -g -Wall

compil: pgp.o encrypt.o decrypt.o commandes.o lire_ecrire.o
	gcc -Wall -o pgp pgp.o encrypt.o decrypt.o commandes.o lire_ecrire.o

pgp.o: pgp.c pgp.h encrypt.h decrypt.h commandes.h lire_ecrire.h types.h
	gcc $(CFLAGS) pgp.c

encrypt.o: encrypt.c pgp.h encrypt.h types.h
	gcc $(CFLAGS) encrypt.c
	
decrypt.o: decrypt.c pgp.h decrypt.h types.h
	gcc $(CFLAGS) decrypt.c
	
commandes.o: commandes.c pgp.h commandes.h types.h
	gcc $(CFLAGS) commandes.c
	
lire_ecrire.o: lire_ecrire.c pgp.h lire_ecrire.h types.h
	gcc $(CFLAGS) lire_ecrire.c
	
indent: 
	indent -linux *.h
	indent -linux *.c
	rm -f *.h~
	rm -f *.c~
	
clean: 
	rm -f pgp
	rm -f *.o
