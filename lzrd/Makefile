

# files and paths

VPATH = src
# PREFIX ?= /usr/local
OBJS  = lzrd.o


# compile settings

CC      = gcc
CFLAGS += -std=c99 -Wall -Wextra -pedantic -ggdb
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
	$(MAKE) -C liboptimize/


.PHONY: etherdream
etherdream:
	$(MAKE) -C libetherdream/


.PHONY: test
test: all test/frame_server.c
	$(CC) $(CFLAGS) $(LIBS) -lm test/frame_server.c -o frame_server


.PHONY: install
install:
	# $(MAKE) -C liboptimize/ install


.PHONY: clean
clean:
	$(MAKE) -C libetherdream/ clean
	$(MAKE) -C liboptimize/ clean
	rm -f *.o lzrd frame_server
