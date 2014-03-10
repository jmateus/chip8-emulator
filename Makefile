CC = gcc
CFLAGS = 

EXECUTABLE = chip8
SOURCES = $(wildcard lib/*.c) $(wildcard src/*.c)
SRC_DIR = src
OBJ_DIR = obj
OBJS = $(patsubst $(SRC_DIR)/%, $(OBJ_DIR)/%, $(SOURCES:.c=.o))
LIBS = -lmingw32 -lSDL2main -lSDL2

TEST_DIR = ./tests
TEST_MK = make --directory=$(TEST_DIR)

HEADERS_MK = ./.headers


all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJS)
	$(CC) $(CFLAGS) -o $(EXECUTABLE) $(OBJS) $(LIBS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@ $(LIBS)

test:
	make
	$(TEST_MK)
	$(TEST_MK) run

clean:
	-rm -f $(OBJ_DIR)/*.o *.exe *.EXE $(HEADERS_MK)
	$(TEST_MK) clean

$(HEADERS_MK): $(SOURCES)
	$(CC) $(CFLAGS) -MM $^ > $(HEADERS_MK)

.PHONY: all clean test

include $(HEADERS_MK)