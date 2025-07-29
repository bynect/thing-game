LIBS=sdl2 SDL2_image

IMGUI = imgui/imgui.cpp \
        imgui/imgui_draw.cpp \
        imgui/imgui_widgets.cpp \
        imgui/imgui_tables.cpp \
        imgui/backends/imgui_impl_sdl2.cpp \
        imgui/backends/imgui_impl_sdlrenderer2.cpp

CXX=g++
CXXFLAGS=-O1 -Wall -g3 $(shell pkgconf --cflags $(LIBS)) -Iimgui -Iimgui/backends
CXXLIBS=$(shell pkgconf --libs $(LIBS))

SRC=$(wildcard *.cpp)
SRC+=$(IMGUI)

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
