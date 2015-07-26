

# files and paths

VPATH = src
# PREFIX ?= /usr/local
OBJS  = main.o


# compile settings

CC=gcc
CFLAGS += -std=c99 -Wall -Wextra -pedantic
CFLAGS += -I./include -I../include
LIBS=-lzmq


# targets

.PHONY: all
all: lzrd


lzrd: optimizer $(OBJS)
	$(CC) -o $@ $(OBJS) $(LIBS)


%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)


.PHONY: optimizer
optimizer:
	cd optimizer/; $(MAKE)


.PHONY: test
test: all
	$(CC) $(CFLAGS) $(LIBS) test/frame_server.c -o frame_server


.PHONY: install
install:
	cd optimizer/; $(MAKE) install


.PHONY: clean
clean:
	cd optimizer/; $(MAKE) clean
	rm -f *.o lzrd frame_server
