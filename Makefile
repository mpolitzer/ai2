CC=gcc

CFLAGS=-Wall -O0 -funroll-loops -c -g -Wno-unused-function -Wno-unused-result
LDFLAGS=-O2
LDLIBS=-lm
SOURCES=main.c game.c bind.c gfx.c heap.c astar.c
HEADERS=game.h bind.h heap.h astar.h

PACKAGES="allegro-5.0 allegro_ttf-5.0 allegro_image-5.0 allegro_dialog-5.0 \
 allegro_primitives-5.0 gl glu swipl"

CFLAGS+=`pkg-config $(PACKAGES) --cflags`
LDLIBS+=`pkg-config $(PACKAGES) --libs`

OBJECTS=$(addsuffix .o, $(basename ${SOURCES}))
EXECUTABLE=main

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)

$(OBJECTS): %.o: %.c $(HEADERS)

clean:
	rm -f $(EXECUTABLE) $(OBJECTS)

.PHONY: all clean
