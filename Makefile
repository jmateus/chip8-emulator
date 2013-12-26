CC = gcc
CFLAGS = 
EXECUTABLE = main
SOURCES = main.c cpu.c memory.c
OBJS = $(SOURCES:.c=.o)
HEADERS_MK = ./.headers

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJS)
	$(CC) $(CFLAGS) -o $(EXECUTABLE) $(OBJS)

.c.o:
	$(CC) $(CFLAGS) -c $<

clean:
	-rm -f *.o *.exe $(HEADERS_MK)

$(HEADERS_MK): $(SOURCES)
	$(CC) $(CFLAGS) -MM $^ >> $(HEADERS_MK)

.PHONY: all clean

include $(HEADERS_MK)