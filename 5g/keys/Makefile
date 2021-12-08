CC = gcc
CFLAGS = -Wall -lcrypto

SRC += 5g_keys.c
SRC += kdf.c
SRC += xres_star.c
SRC += hxres_star.c
SRC += k_ausf.c
SRC += k_seaf.c
SRC += k_amf.c
SRC += k_nas_int.c
SRC += k_nas_enc.c
SRC += k_gnb.c
SRC += k_nh.c
SRC += k_rrc_int.c
SRC += k_rrc_enc.c
SRC += k_up_enc.c
SRC += k_up_int.c
SRC += k_ng_ran_star.c

OBJ = $(SRC:.c=.o)


all: 5g_keys

5g_keys: $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@

%.o: %.c kdf.h
	$(CC) -Wall -c $< -o $@

clean:
	rm -f 5g_keys $(OBJ)
