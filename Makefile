CFLAGS=-c -g -Wall

compil: pgp.o envoi.o reception.o commandes.o lire_ecrire.o
	gcc -Wall -o pgp pgp.o envoi.o reception.o commandes.o lire_ecrire.o

pgp.o: pgp.c pgp.h envoi.h reception.h commandes.h lire_ecrire.h types.h
	gcc $(CFLAGS) pgp.c

envoi.o: envoi.c pgp.h envoi.h lire_ecrire.h types.h
	gcc $(CFLAGS) envoi.c
	
reception.o: reception.c pgp.h reception.h types.h
	gcc $(CFLAGS) reception.c
	
commandes.o: commandes.c pgp.h commandes.h lire_ecrire.h types.h
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
