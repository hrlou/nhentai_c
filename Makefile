PREFIX=/usr/local
CXX=c++
CXXFLAGS=-std=c++17 -Wall -pedantic -O3
INCLUDE=-Iinclude -I.
LDFLAGS=-lcurl -lzip -lstdc++fs

CXXFILES=$(patsubst src/%.cpp,%.o,$(wildcard src/*.cpp))

nhentai: main.cpp $(CXXFILES)
	$(CXX) $(INCLUDE) $^ -o $@ $(CXXFLAGS) $(LDFLAGS)
%.o: src/%.cpp include/%.hpp
	$(CXX) $(INCLUDE) -c $< -o $@ $(CXXFLAGS)
clean:
	@rm -f nhentai *.o
install: nhentai
	install -m 775 nhentai $(PREFIX)/bin/