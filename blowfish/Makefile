CC = gcc
CFLAGS = -c -Wall -Werror -O2 -g

default: cipher

cipher.o: cipher.c blowfish.h builds.h
	$(CC) $(CFLAGS) cipher.c

bf_cfb64.o: bf_cfb64.c blowfish.h bf_locl.h
	$(CC) $(CFLAGS) bf_cfb64.c

bf_enc.o: bf_enc.c blowfish.h bf_locl.h
	$(CC) $(CFLAGS) bf_enc.c

bf_skey.o: bf_skey.c blowfish.h bf_locl.h bf_pi.h
	$(CC) $(CFLAGS) bf_skey.c

cipher: cipher.o bf_cfb64.o bf_enc.o bf_skey.o
	$(CC) -o cipher cipher.o bf_cfb64.o bf_enc.o bf_skey.o

clean:
	$(RM) count *.o *~
