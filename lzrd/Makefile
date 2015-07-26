

# files and paths

VPATH = src
# PREFIX ?= /usr/local
OBJS  = lzrd.o


# compile settings

CC      = gcc
CFLAGS += -std=c99 -Wall -Wextra -pedantic
CFLAGS += -I./include -I../include
LIBS    = -lzmq -lpthread


# targets

.PHONY: all
all: lzrd


lzrd: optimizer etherdream $(OBJS)
	$(CC) $(OBJS) ./libetherdream/etherdream.a $(LIBS) -o $@


%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)


.PHONY: optimizer
optimizer:
	cd liboptimize/; $(MAKE)


.PHONY: etherdream
etherdream:
	cd libetherdream/; $(MAKE)


.PHONY: test
test: all
	$(CC) $(CFLAGS) $(LIBS) test/frame_server.c -o frame_server


.PHONY: install
install:
	# cd liboptimize/; $(MAKE) install


.PHONY: clean
clean:
	cd libetherdream/; $(MAKE) clean
	cd liboptimize/; $(MAKE) clean
	rm -f *.o lzrd frame_server
