CC = gcc

APPS += global_gnb_id
APPS += global_ng_enb_id
APPS += nr_cgi
APPS += tai
APPS += guami
APPS += 5g_s_tmsi


all: $(APPS)

global_gnb_id: global_gnb_id.o util.o
	$(CC) $^ -o $@

global_ng_enb_id: global_ng_enb_id.o util.o
	$(CC) $^ -o $@

nr_cgi: nr_cgi.o util.o
	$(CC) $^ -o $@

tai: tai.o util.o
	$(CC) $^ -o $@

guami: guami.o util.o
	$(CC) $^ -o $@

5g_s_tmsi: 5g_s_tmsi.o util.o
	$(CC) $^ -o $@

%.o: %.c util.h
	$(CC) -Wall -c $< -o $@

clean:
	rm -f $(APPS) *.o
