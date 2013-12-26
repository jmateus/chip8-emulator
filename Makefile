CC = gcc
CFLAGS = 
EXECUTABLE = main
SOURCES = main.c cpu.c memory.c graphics.c
OBJS = $(SOURCES:.c=.o)
LIBS = -lmingw32 -lSDL2main -lSDL2
HEADERS_MK = ./.headers

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJS)
	$(CC) $(CFLAGS) -o $(EXECUTABLE) $(OBJS) $(LIBS)

.c.o:
	$(CC) $(CFLAGS) -c $< $(LIBS)

clean:
	-rm -f *.o *.exe $(HEADERS_MK)

$(HEADERS_MK): $(SOURCES)
	$(CC) $(CFLAGS) -MM $^ > $(HEADERS_MK)

.PHONY: all clean

include $(HEADERS_MK)