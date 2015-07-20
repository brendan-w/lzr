


# files and paths

EXE_NAME=lzr_optimize

HEADERS = include/lzr_optimize.h include/internals.h ../include/lzr.h
OBJ =     src/lzr_optimize.o src/find_paths.o src/rearrange_paths.o

#src/compile_paths.o


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
	rm src/*.o
	rm $(EXE_NAME)

