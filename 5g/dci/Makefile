CC = gcc
CFLAGS = -Wall
LDFLAGS = -Wall


APPS = dci_decode_0_0 dci_decode_1_0 dci_bits

all: $(APPS)

dci_decode_0_0: dci_decode_0_0.o utility.o
	$(CC) -o $@ $(LDFLAGS) $^

dci_decode_1_0: dci_decode_1_0.o utility.o
	$(CC) -o $@ $(LDFLAGS) $^

dci_bits: dci_bits.o utility.o
	$(CC) -o $@ $(LDFLAGS) -lm $^

%.o: %.c utility.h
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(APPS)
	rm -f *.o 
