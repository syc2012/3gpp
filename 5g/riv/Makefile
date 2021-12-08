CC = gcc
CFLAGS = -Wall

all: riv_enc riv_dec

riv_enc: riv_enc.c
	$(CC) $(CFLAGS) $< -lm -o $@

riv_dec: riv_dec.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f riv_enc riv_dec
