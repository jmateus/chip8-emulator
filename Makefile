CC = gcc
CFLAGS = 
EXECUTABLE = chip8
SOURCES = memory.c graphics.c input.c sound.c cpu.c main.c
OBJS = $(SOURCES:.c=.o)
LIB_OBJS = lib/simpleini.o
LIBS = -lmingw32 -lSDL2main -lSDL2
HEADERS_MK = ./.headers

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJS)
	$(CC) $(CFLAGS) -o $(EXECUTABLE) $(OBJS) $(LIB_OBJS) $(LIBS)

.c.o:
	$(CC) $(CFLAGS) -c $< $(LIBS)

clean:
	-rm -f *.o *.exe *.EXE $(HEADERS_MK)

$(HEADERS_MK): $(SOURCES)
	$(CC) $(CFLAGS) -MM $^ > $(HEADERS_MK)

.PHONY: all clean

include $(HEADERS_MK)