compile:
	gcc -c aes.c
	gcc aes.h
	gcc -c utils.c
	gcc utils.h

clean:
	-rm aes.o
	-rm aes.h.gch
	-rm utils.o
	-rm utils.h.gch