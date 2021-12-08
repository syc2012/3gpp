CC = gcc
CFLAGS = -Wall

all: band_FR1 band_FR2 nr_arfcn gscn

band_RF1: band_FR1.c
	$(CC) $(CFLAGS) $< -o $@

band_RF2: band_FR2.c
	$(CC) $(CFLAGS) -lm $< -o $@

nr_arfcn: nr_arfcn.c
	$(CC) $(CFLAGS) $< -o $@

gscn: gscn.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f band_FR1 band_FR2 nr_arfcn gscn
