# openssl-1.1.1c
OPENSSL = /usr/local

CC = gcc

CFLAGS += -Wall
CFLAGS += -I$(OPENSSL)/include

LDFLAGS += -L$(OPENSSL)/lib
LDFLAGS += -lssl
LDFLAGS += -lcrypto


APP_NAME = ecies_test

SRC += ecies_api.c
SRC += ecies_core.c
SRC += utility.c
SRC += main.c

DEP = ecies_api.h ecies_core.h utility.h

OBJS = $(SRC:.c=.o)

all: $(APP_NAME)

$(APP_NAME): $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -o $@

%.o: %.c $(DEP)
	$(CC) $(CFLAGS) -c $< -o $@
    
clean:
	@rm -f $(APP_NAME) $(OBJS)

