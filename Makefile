PREFIX = "/usr/local"
CC = "gcc"
CFLAGS = "-Iinclude" "-std=c99" "-O2" "-Wall"

nhentai: main.o tags.o download.o
	$(CC) $(CFLAGS) -o nhentai main.o tags.o download.o -lcurl 

main.o: src/main.c include/main.h config.def.h
	$(CC) $(CFLAGS) -c src/main.c

tags.o: src/tags.c include/tags.h
	$(CC) $(CFLAGS) -c src/tags.c 
	
download.o: src/download.c include/download.h
	$(CC) $(CFLAGS) -c src/download.c


clean:
	rm -f *.o nhentai

install: nhentai 
	mkdir -p $(PREFIX)/bin
	cp -f nhentai $(PREFIX)/bin
	cp -f scripts/nhentai_rename.sh $(PREFIX)/bin/nhentai_rename
	cp -f scripts/nhentai_gen.sh $(PREFIX)/bin/nhentai_gen
	cp -f scripts/nhentai_search.sh $(PREFIX)/bin/nhentai_search
	cp -f scripts/nhentai_db.sh $(PREFIX)/bin/nhentai_db
	cp -f scripts/nhentai_cbz.sh $(PREFIX)/bin/nhentai_cbz
	chmod +x $(PREFIX)/bin/nhentai
	chmod +x $(PREFIX)/bin/nhentai_rename
	chmod +x $(PREFIX)/bin/nhentai_gen
	chmod +x $(PREFIX)/bin/nhentai_search
	chmod +x $(PREFIX)/bin/nhentai_db
	chmod +x $(PREFIX)/bin/nhentai_cbz

uninstall:
	rm -f $(PREFIX)/bin/nhentai
	rm -f $(PREFIX)/bin/nhentai_rename
	rm -f $(PREFIX)/bin/nhentai_gen
	rm -f $(PREFIX)/bin/nhentai_search
	rm -f $(PREFIX)/bin/nhentai_db
	rm -f $(PREFIX)/bin/nhentai_cbz
