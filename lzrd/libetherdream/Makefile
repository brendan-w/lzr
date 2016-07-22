
CC      = gcc
CFLAGS  = -Wall -Wextra -ansi -pedantic -std=c99 -fPIC
CFLAGS += -lpthread
ARFLAGS = rvs

OBJS = etherdream.o


UNAME = $(shell uname)

ifeq ($(UNAME), Darwin)

all: etherdream.dylib

etherdream.dylib: etherdream.c
	gcc $(CFLAGS) -dynamiclib etherdream.c -o etherdream.dylib

else
CFLAGS += -lrt
endif


.PHONY: all
all: etherdream.a

debug: CFLAGS += -DDEBUG
debug: etherdream.a

etherdream.a: $(OBJS)
	$(AR) $(ARFLAGS) $@ $^


%.o: %.c
	$(CC) -c $< $(CFLAGS) -o $@


test: etherdream.a test.c
	$(CC) $(CFLAGS) -lm test.c ./etherdream.a -o $@


.PHONY: clean
clean:
	rm -f *.dylib *.o *.a test test.dSYM
