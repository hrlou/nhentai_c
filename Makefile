PREFIX = "/usr/local"
CC = "gcc"
INCLUDE="-Iinclude"
CFLAGS = "-std=c99" "-O3" "-Wall"
C=$(CC) $(INCLUDE) $(CFLAGS)

nhentai: main.o tags.o download.o
	$(C) -o $@ $^ -lcurl 

main.o: src/main.c include/main.h config.def.h
	$(C) -c $< -o $@

tags.o: src/tags.c include/tags.h
	$(C) -c $< -o $@ 
	
download.o: src/download.c include/download.h
	$(C) -c $< -o $@

clean:
	rm -f *.o nhentai

install: nhentai 
	mkdir -p $(PREFIX)/bin
	cp -f nhentai $(PREFIX)/bin
	cp -f scripts/nhentai_gen.sh $(PREFIX)/bin/nhentai_gen
	cp -f scripts/nhentai_search.sh $(PREFIX)/bin/nhentai_search
	cp -f scripts/nhentai_cbz.sh $(PREFIX)/bin/nhentai_cbz
	chmod +x $(PREFIX)/bin/nhentai
	chmod +x $(PREFIX)/bin/nhentai_gen
	chmod +x $(PREFIX)/bin/nhentai_search
	chmod +x $(PREFIX)/bin/nhentai_cbz

uninstall:
	rm -f $(PREFIX)/bin/nhentai*
