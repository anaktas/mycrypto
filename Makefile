compile:
	gcc utils.h utils.c aes.h aes.c main.c -o aes

clean:
	-rm aes
	-rm encrypted
	-rm decrypted