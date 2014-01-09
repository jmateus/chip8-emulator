CC = gcc
CFLAGS = 
SOURCES = simpleini.c
OBJS = $(SOURCES:.c=.o)

all: $(OBJS)

.c.o:
	$(CC) $(CFLAGS) -c $<

clean:
	-rm -f *.o *.exe *.EXE

.PHONY: all clean
