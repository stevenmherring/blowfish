#SRC=bf_cfb64.c \
#	bf_enc.c \
#	bf_skey.c \
#	cipher.c
#
#HDRS=bf_pi.h bf_locl.h
#
#cipher: $(SRC) $(HDRS)
#	$(CC) $(LDFLAGS) -c $(CFLAGS) -o cipher $(SRC)

CC = gcc
CFLAGS = -Wall -Werror -O2 -g
LDFLAGS=-lwrap -static

# typing 'make' will invoke the first target entry in the file
# (in this case the default target entry)
# you can name this target entry anything, but "default" or "all"
# are the most commonly used names by convention
#
default: cipher

clean:
	$(RM) count *.o *~



all: cipher

cipher.o: cipher.c
	gcc -c $(CFLAGS) cipher.c

bf_enc.o: bf_enc.c
	gcc -c bf_enc.c

bf_skey.o: bf_skey.c
	gcc -c bf_skey.c

bf_cfb64.o: bf_cfb64.c
	gcc -c bf_cfb64.c

cipher: cipher.o bf_enc.o bf_skey.o bf_cfb64.o
	gcc -o cipher cipher.o bf_enc.o bf_skey.o bf_cfb64.o
