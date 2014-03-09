CC = gcc
CFLAGS = 
EXECUTABLE = chip8
SOURCES = $(wildcard lib/*.c) $(wildcard *.c)
OBJS = $(SOURCES:.c=.o)
LIBS = -lmingw32 -lSDL2main -lSDL2
HEADERS_MK = ./.headers

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJS)
	$(CC) $(CFLAGS) -o $(EXECUTABLE) $(OBJS) $(LIBS)

.c.o:
	$(CC) $(CFLAGS) -c $< -o $*.o $(LIBS)

test:
	make --directory=./tests
	make --directory=./tests run

clean:
	-rm -f *.o *.gch *.exe *.EXE $(HEADERS_MK)

$(HEADERS_MK): $(SOURCES)
	$(CC) $(CFLAGS) -MM $^ > $(HEADERS_MK)

.PHONY: all clean test

include $(HEADERS_MK)