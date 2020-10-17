CC = cc
PREFIX = "/usr/local"

nhentai: main.o download.o
	$(CC) main.o -lcurl download.o -o nhentai

main.o: main.c options.h main.h
	$(CC) -c main.c
	
download.o: download.c download.h main.h
	$(CC) -c download.c

clean:
	rm *.o nhentai
	
install: nhentai
	mkdir -p $(PREFIX)/bin
	cp -f nhentai $(PREFIX)/bin
	chmod 755 $(PREFIX)/bin/nhentai

uninstall:
	rm -f $(PREFIX)/bin/nhentai
