CC=gcc

CFLAGS=-Wall -O0 -funroll-loops -c -g -Wno-unused-function -Wno-unused-result
LDFLAGS=-O2
LDLIBS=-lm
SOURCES=main.c game.c bind.c
HEADERS=game.h bind.h

CFLAGS+=`pkg-config swipl --cflags`
LDLIBS+=`pkg-config swipl --libs`

OBJECTS=$(addsuffix .o, $(basename ${SOURCES}))
EXECUTABLE=main

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)

$(OBJECTS): %.o: %.c $(HEADERS)

clean:
	rm -f $(EXECUTABLE) $(OBJECTS)

.PHONY: all clean
