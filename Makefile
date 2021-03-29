PREFIX=/usr/local
CC=gcc
INCLUDE=-Iinclude
CFLAGS=-std=c99 -O3 -Wall
LINK=-lcurl -lzip

CFILES=$(patsubst src/%.c,%.o,$(wildcard src/*.c))

nhentai: $(CFILES)
	$(CC) $(INCLUDE) $^ -o $@ $(CFLAGS) $(LINK)
%.o: src/%.c include/%.h config.def.h
	$(CC) $(INCLUDE) -c $< -o $@ $(CFLAGS)
clean:
	@rm -f nhentai *.o
install: nhentai
	@cp -f nhentai $(PREFIX)/bin/nhentai
