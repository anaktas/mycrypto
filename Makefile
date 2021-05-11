compile:
	gcc -c aes.c
	gcc aes.h

clean:
	-rm aes.o
	-rm aes.h.gch