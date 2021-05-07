PREFIX=/usr/local
CXX=c++
CXXFLAGS=-std=c++11 -Wall -pedantic -O3
INCLUDE=-Iinclude -Iexternal -I.
LDFLAGS=-lcurl -lzip -lpthread

.PHONY: all clean
all: post-build
pre-build:
ifeq ($(wildcard ./external/nlohmann/json.hpp),)
	mkdir -p ./external/nlohmann
	curl -L 'https://github.com/nlohmann/json/releases/download/v3.9.1/json.hpp' -o ./external/nlohmann/json.hpp
endif
post-build: main-build

main-build: pre-build
	@$(MAKE) --no-print-directory nhentai

nhentai: curl.o out.o nhentai.o main.o
	$(CXX) $(INCLUDE) $^ -o $@ $(CXXFLAGS) $(LDFLAGS)
curl.o: src/curl.cpp config.h include/curl.hpp include/out.hpp include/utils.hpp
	$(CXX) $(INCLUDE) -c $< -o $@ $(CXXFLAGS)
out.o: src/out.cpp config.h include/out.hpp include/curl.hpp include/nhentai.hpp include/utils.hpp
	$(CXX) $(INCLUDE) -c $< -o $@ $(CXXFLAGS)
nhentai.o: src/nhentai.cpp config.h include/out.hpp include/curl.hpp include/nhentai.hpp include/utils.hpp
	$(CXX) $(INCLUDE) -c $< -o $@ $(CXXFLAGS)
main.o: main.cpp
	$(CXX) $(INCLUDE) -c $< -o $@ $(CXXFLAGS)
clean:
	rm -rf external
	rm -f nhentai *.o
install: nhentai
	install -m 775 nhentai -d $(PREFIX)/bin/ nhentai