debug:
	gcc -g config.h log.h log.c utils.h utils.c aes.h aes.c main.c -o aes
	ulimit -c unlimited

release:
	gcc config.h log.h log.c utils.h utils.c aes.h aes.c main.c -o aes

clean:
	-rm aes
	-rm encrypted
	-rm decrypted
	-rm core

reset:
	-rm encrypted
	-rm decrypted
	clear