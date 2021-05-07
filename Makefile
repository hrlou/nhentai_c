PREFIX=/usr/local
CXX=c++
CXXFLAGS=-std=c++17 -Wall -pedantic -O3
INCLUDE=-Iinclude -I.
LDFLAGS=-lcurl -lzip -lstdc++fs

CXXFILES=$(patsubst src/%.cpp,%.o,$(wildcard src/*.cpp))

nhentai: main.o $(CXXFILES)
	$(CXX) $(INCLUDE) $^ -o $@ $(CXXFLAGS) $(LDFLAGS)
main.o: main.cpp
	$(CXX) $(INCLUDE) -c $< -o $@ $(CXXFLAGS)
%.o: src/%.cpp include/%.hpp
	$(CXX) $(INCLUDE) -c $< -o $@ $(CXXFLAGS)
clean:
	@rm -f nhentai *.o
install: nhentai
	install -m 775 nhentai $(PREFIX)/bin/
