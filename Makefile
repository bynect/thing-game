CXX=g++
CXXFLAGS=-O1 -Wall -g3
CXXLIBS=-lSDL2 -lSDL2_image

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
