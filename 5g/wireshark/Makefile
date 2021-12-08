CC = gcc

all: wireshark_mac wireshark_rlc wireshark_pdcp

wireshark_mac: wireshark_mac.c packet-mac-nr.h
	$(CC) -Wall $< -o $@

wireshark_rlc: wireshark_rlc.c packet-rlc-nr.h
	$(CC) -Wall $< -o $@

wireshark_pdcp: wireshark_pdcp.c packet-pdcp-nr.h packet-rohc.h
	$(CC) -Wall $< -o $@

clean:
	rm wireshark_mac wireshark_rlc wireshark_pdcp
