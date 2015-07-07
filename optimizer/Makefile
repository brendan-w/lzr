


# files and paths

EXE_NAME=optimize

HEADERS = include/optimize.h include/internals.h ../include/lzr.h
OBJ =     src/optimize.o src/find_paths.o src/rearrange_paths.o src/compile_paths.o


# compile settings

CC=gcc
CFLAGS=-std=c99 -Wall -Wextra -pedantic -ggdb -I./include -I../include
LIBS=-lm



# targets

$(EXE_NAME): $(OBJ) 
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

%.o: %.c $(HEADERS)
	$(CC) -c -o $@ $< $(CFLAGS)

.PHONY: clean
clean:
	rm -f $(OBJ) $(EXE_NAME)

