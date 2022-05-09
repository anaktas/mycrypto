debug:
	make clean
	gcc -g config.h log.h log.c utils.h utils.c aes.h aes.c crypto_operations.h crypto_operations.c main.c -o aes
	ulimit -c unlimited

release:
	make clean
	gcc config.h log.h log.c utils.h utils.c aes.h aes.c crypto_operations.h crypto_operations.c main.c -o aes

clean:
	-rm aes
	-rm encrypted
	-rm decrypted
	-rm core
	clear

reset:
	-rm encrypted
	-rm decrypted
	clear
