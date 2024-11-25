CC = gcc
CFLAGS = -Wall

all: time_loc freq_loc sync_raster pointA mib_dec mib_enc

time_loc: time_loc.c
	$(CC) $(CFLAGS) $< -o $@

freq_loc: freq_loc.c
	$(CC) $(CFLAGS) $< -o $@

sync_raster: sync_raster.c
	$(CC) $(CFLAGS) $< -o $@

pointA: pointA.c
	$(CC) $(CFLAGS) $< -o $@

mib_dec: mib_dec.c
	$(CC) $(CFLAGS) $< -o $@

mib_enc: mib_enc.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f time_loc freq_loc sync_raster pointA mib_dec mib_enc
