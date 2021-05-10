PREFIX=/usr/local
CXX=c++
CXXFLAGS=-pipe -std=c++11 -Wall -pedantic -O3
INCLUDE=-Iinclude -Iexternal -I. -I$(PREFIX)/include
LDFLAGS=-lpthread -lcurl -lzip

.PHONY: all clean
all: post-build
pre-build:
ifeq ($(wildcard $(PREFIX)/include/nlohmann/json.hpp),)
ifeq ($(wildcard ./external/nlohmann/json.hpp),)
	mkdir -p ./external/nlohmann
	curl -L 'https://github.com/nlohmann/json/releases/download/v3.9.1/json.hpp' -o ./external/nlohmann/json.hpp
endif
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
ifeq ($(wildcard ./external),)
	rm -rf external
endif
	rm -f nhentai *.o
install: nhentai
	install -D external/nlohmann/json.hpp $(PREFIX)/include/nlohmann/json.hpp
	install -m 775 nhentai $(PREFIX)/bin/nhentai