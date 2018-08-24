all: server

CC ?= gcc
SRC = redeye_server.c config.c database.c handler.c inih/ini.c
CFLAGS = -Iinclude
LDFLAGS = -lulfius -g -ljansson -lsqlite3 -pthread

ulfius_lib: ulfius
	cd ulfius/src
	make static

server: $(SRC)
	$(CC) $^ -o $@ $(CFLAGS) $(LDFLAGS)

clean:
	rm -f server
#	rm -rf libmicrohttpd*
