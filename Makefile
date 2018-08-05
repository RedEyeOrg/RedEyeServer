all: server

CC ?= gcc
SRC = redeye_server.c config.c inih/ini.c
CFLAGS = -Iinclude
LDFLAGS = -lmicrohttpd -g


server: $(SRC)
	$(CC) $^ -o $@ $(CFLAGS) $(LDFLAGS)

clean:
	rm -f server
#	rm -rf libmicrohttpd*
