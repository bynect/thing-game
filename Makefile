LIBS=sdl2 SDL2_image SDL2_ttf

CXX=g++
CXXFLAGS=-O1 -Wall -g3 $(shell pkgconf --cflags $(LIBS))
CXXLIBS=$(shell pkgconf --libs $(LIBS))

SRC=$(wildcard *.cpp)
OBJ=$(patsubst %.cpp,%.o,$(SRC))
EXE=game.bin

all: $(EXE)

$(EXE): $(OBJ)
	$(CXX) $(CXXLIBS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -o $@ -c $^

.PHONY: clean
clean:
	rm -f $(EXE) $(OBJ)
