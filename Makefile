CC = cc
PREFIX = "/usr/local"

nhentai: main.o download.o
	$(CC) main.o -lcurl download.o -o nhentai

main.o: src/main.c src/main.h config.def.h
	$(CC) -c src/main.c
	
download.o: src/download.c src/download.h
	$(CC) -c src/download.c

clean:
	rm *.o nhentai
	
install: nhentai
	mkdir -p $(PREFIX)/bin
	cp -f nhentai $(PREFIX)/bin
	chmod 755 $(PREFIX)/bin/nhentai

uninstall:
	rm -f $(PREFIX)/bin/nhentai
