CC = gcc
APPS = eap_sim eap_aka eap_aka_prime

all: $(APPS)

eap_sim: eap_sim.o kdf.o
	$(CC) -Wall -lcrypto $^ -o $@

eap_aka: eap_aka.o kdf.o
	$(CC) -Wall -lcrypto $^ -o $@

eap_aka_prime: eap_aka_prime.o kdf.o
	$(CC) -Wall -lcrypto $^ -o $@

%.o: %.c kdf.h
	$(CC) -Wall -c $<

clean:
	@rm -f $(APPS)
	@rm -f *.o

.PHONY: all clean
