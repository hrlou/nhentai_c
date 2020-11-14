PREFIX = "/usr/local"
CC = "gcc"
CFLAGS = "-O2"

nhentai: main.o download.o
	$(CC) $(CFLAGS) main.o -lcurl download.o -o nhentai

main.o: src/main.c src/main.h config.def.h
	$(CC) $(CFLAGS) -Wformat=0 -c src/main.c
	
download.o: src/download.c src/download.h
	$(CC) $(CFLAGS) -c src/download.c

clean:
	rm *.o nhentai

scripts: scripts/nhentai_rename.sh scripts/nhentai_gen.sh
	cp -f scripts/nhentai_rename.sh $(PREFIX)/bin/nhentai_rename
	cp -f scripts/nhentai_gen.sh $(PREFIX)/bin/nhentai_gen
	chmod 755 $(PREFIX)/bin/nhentai_rename
	chmod 755 $(PREFIX)/bin/nhentai_gen


install: nhentai scripts
	mkdir -p $(PREFIX)/bin
	cp -f nhentai $(PREFIX)/bin
	chmod 755 $(PREFIX)/bin/nhentai

uninstall:
	rm -f $(PREFIX)/bin/nhentai
	rm -f $(PREFIX)/bin/nhentai_rename
	rm -f $(PREFIX)/bin/nhentai_gen
