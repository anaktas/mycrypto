compile:
	gcc -g utils.h utils.c aes.h aes.c main.c -o aes
	ulimit -c unlimited

clean:
	-rm aes
	-rm encrypted
	-rm decrypted
	-rm core