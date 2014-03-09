CC = gcc
CFLAGS = 
SOURCES = $(wildcard *.c)
OBJS = $(SOURCES:.c=.o)
TESTS = $(SOURCES:.c=)
FILES = $(wildcard ../*.o) $(wildcard ../lib/*.o)
LIBS = -lmingw32 -lSDL2main -lSDL2
HEADERS_MK = ./.headers

all: $(TESTS)

$(TESTS): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $@.o $(FILES) $(LIBS)

%.o:
	$(CC) $(CFLAGS) -c $*.c $(LIBS)

run:
	$(foreach test,$(TESTS), ./$(test) -s;)

clean:
	-rm -f *.o *.exe *.EXE $(HEADERS_MK)

$(HEADERS_MK): $(SOURCES)
	$(CC) $(CFLAGS) -MM $^ > $(HEADERS_MK)

.PHONY: all clean run

include $(HEADERS_MK)
