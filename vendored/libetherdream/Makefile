CC = gcc
CFLAGS = -I../../common -Wall -Wextra -ansi -pedantic -std=c99 -lm -lpthread

UNAME = $(shell uname)

ifneq ($(UNAME), Darwin)
CFLAGS += -lrt
endif

all: test

ifeq ($(UNAME), Darwin)
all: etherdream.dylib
endif

test: etherdream.c etherdream.h test.c
	$(CC) $(CFLAGS) -g etherdream.c test.c -o $@

etherdream.dylib: etherdream.c
	gcc $(CFLAGS) -dynamiclib etherdream.c -o etherdream.dylib

.PHONY: clean

clean:
	rm -rf etherdream.dylib etherdream.c.* test test.dSYM
