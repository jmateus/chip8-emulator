CC = gcc
CFLAGS = 
SOURCES = $(wildcard *.c)
OBJS = $(SOURCES:.c=.o)
TESTS = $(SOURCES:.c=)
LIBS = -lmingw32 -lSDL2main -lSDL2

ALL_FILES = $(wildcard ../obj/*.o) $(wildcard ../lib/*.o)
FILES = $(filter-out ../obj/main.o,$(ALL_FILES))

HEADERS_MK = ./.headers


all: $(TESTS)

$(TESTS): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $@.o $(FILES) $(LIBS)

.c.o:
	$(CC) $(CFLAGS) -c $*.c $(LIBS)

run:
	$(foreach test,$(TESTS), ./$(test) -s;)

clean:
	-rm -f *.o *.gch *.exe *.EXE $(HEADERS_MK)

$(HEADERS_MK): $(SOURCES)
	$(CC) $(CFLAGS) -MM $^ > $(HEADERS_MK)

.PHONY: all clean run

include $(HEADERS_MK)
