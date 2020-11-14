PREFIX = "/usr/local"
CC = "gcc"
CFLAGS = "-O2"

nhentai: main.o search.o parse.o download.o
	$(CC) $(CFLAGS) main.o search.o parse.o -lcurl download.o -o nhentai

main.o: src/main.c src/main.h config.def.h
	$(CC) $(CFLAGS) -c src/main.c

search.o: src/search.c src/search.h
	$(CC) $(CFLAGS) -c src/search.c

parse.o: src/parse.c src/parse.h config.def.h
	$(CC) $(CFLAGS) -c src/parse.c
	
download.o: src/download.c src/download.h
	$(CC) $(CFLAGS) -c src/download.c


clean:
	rm *.o nhentai

install: nhentai 
	mkdir -p $(PREFIX)/bin
	cp -f nhentai $(PREFIX)/bin
	chmod 755 $(PREFIX)/bin/nhentai
	cp -f scripts/nhentai_rename.sh $(PREFIX)/bin/nhentai_rename
	cp -f scripts/nhentai_gen.sh $(PREFIX)/bin/nhentai_gen
	cp -f scripts/nhentai_search.sh $(PREFIX)/bin/nhentai_search
	chmod 755 $(PREFIX)/bin/nhentai_rename
	chmod 755 $(PREFIX)/bin/nhentai_gen
	chmod 755 $(PREFIX)/bin/nhentai_search

uninstall:
	rm -f $(PREFIX)/bin/nhentai
	rm -f $(PREFIX)/bin/nhentai_rename
	rm -f $(PREFIX)/bin/nhentai_gen
	rm -f $(PREFIX)/bin/nhentai_search
