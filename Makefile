CC = cc
PREFIX = "/usr/local"

nhentai: main.o download.o
	$(CC) main.o -lcurl download.o -o nhentai

main.o: src/main.c src/main.h config.def.h
	$(CC) -Wformat=0 -c src/main.c
	
download.o: src/download.c src/download.h
	$(CC) -c src/download.c

clean:
	rm *.o nhentai
	
install: nhentai
	mkdir -p $(PREFIX)/bin
	cp -f nhentai $(PREFIX)/bin
	cp -f scripts/nhentai_rename.sh $(PREFIX)/bin/nhentai_rename
	cp -f scripts/nhentai_gen.sh $(PREFIX)/bin/nhentai_gen
	chmod 755 $(PREFIX)/bin/nhentai
	chmod 755 $(PREFIX)/bin/nhentai_rename
	chmod 755 $(PREFIX)/bin/nhentai_gen

uninstall:
	rm -f $(PREFIX)/bin/nhentai
	rm -f $(PREFIX)/bin/nhentai_rename
	rm -f $(PREFIX)/bin/nhentai_gen
