


# files and paths

EXE_NAME=optimize

HEADERS=\
	../include/lzr_types.h \
	src/optimize.h \
	src/find_paths.h

OBJ=\
	src/main.o \
	src/find_paths.o


# compile settings

CC=gcc
CFLAGS=-std=c99 -Wall -Wextra -pedantic -ggdb -I../include
LIBS=-lm



# targets

$(EXE_NAME): $(OBJ) 
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

%.o: %.c $(HEADERS)
	$(CC) -c -o $@ $< $(CFLAGS)

.PHONY: clean

clean:
	rm -f $(OBJ) $(EXE_NAME)

