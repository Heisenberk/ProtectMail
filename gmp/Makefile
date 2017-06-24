CFLAGS=-c -g -Wall

test: clean compil
	./gmp

compil: clean gmp.o 
	gcc -Wall -o gmp gmp.o

gmp.o: gmp.c gmp.h
	gcc $(CFLAGS) gmp.c
	
clean: 
	rm -f *.o
	rm -f gmp
