

# files and paths

VPATH = src
PREFIX ?= /usr/local
OBJS  = lzr_optimize.o find_paths.o rearrange_paths.o compile_paths.o


# compile settings

CC=gcc
CFLAGS += -std=c99 -Wall -Wextra -pedantic -fPIC
CFLAGS += -I./include -I../../include
LIBS=-lm

ARFLAGS=rvs


# targets

.PHONY: all
all: liblzr_optimize.a liblzr_optimize.so


liblzr_optimize.so: $(OBJS)
	$(CC) -shared -o $@ $^ $(LIBS)


liblzr_optimize.a: $(OBJS) 
	$(AR) $(ARFLAGS) $@ $^


%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)


.PHONY: test
test: all
	$(CC) $(CFLAGS) test/test.c ./liblzr_optimize.so -o test-optimizer
	./test-optimizer


.PHONY: install
install:
	install -m 0755 liblzr_optimize.a $(PREFIX)/lib
	install -m 0755 liblzr_optimize.so $(PREFIX)/lib
	install -m 0755 include/lzr_optimize.h $(PREFIX)/include


.PHONY: clean
clean:
	rm -f *.a *.o *.so test-optimizer
