CXX=c++
CXXFLAGS=-std=c++11 -pipe -pedantic -O3 -Wall
PREFIX=/usr/local
INCLUDE=-I. -I./include -I$(BUILD_DIR) -I$(PREFIX)/include
LDFLAGS=-lpthread -lcurl -lzip

BIN=nhentai
BUILD_DIR=./build

CPP=$(wildcard src/*.cpp)
OBJ=$(CPP:%.cpp=$(BUILD_DIR)/%.o)
DEP=$(OBJ:%.o=%.d)

.PHONY: all clean

all: post-build

pre-build:
ifeq ($(wildcard $(PREFIX)/include/nlohmann/json.hpp),)
ifeq ($(wildcard $(BUILD_DIR)/json.hpp),)
	@mkdir -p $(BUILD_DIR)
	-curl -L 'https://github.com/nlohmann/json/releases/download/v3.9.1/json.hpp' -o $(BUILD_DIR)/json.hpp
endif
endif

post-build: main-build

main-build: pre-build
	@$(MAKE) --no-print-directory $(BIN)

$(BIN) : $(BUILD_DIR)/$(BIN)
	-ln -sf $< $@

$(BUILD_DIR)/$(BIN) : $(OBJ)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(INCLUDE) $^ -o $@ $(LDFLAGS)

-include $(DEP)

$(BUILD_DIR)/%.o : %.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(INCLUDE) -MMD -c $< -o $@

clean:
	-rm -rf $(BIN) $(OBJ) $(DEP) $(BUILD_DIR)

install: all
ifeq ($(wildcard $(BUILD_DIR)/json.hpp),)
	-install -D $(BUILD_DIR)/json.hpp $(PREFIX)/include/nlohmann/json.hpp
endif
	-install -m 775 $(BIN) $(PREFIX)/bin/$(BIN)