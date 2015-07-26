

# files and paths

VPATH = src
# PREFIX ?= /usr/local
OBJS  = main.o


# compile settings

CC      = gcc
CFLAGS += -std=c99 -Wall -Wextra -pedantic
CFLAGS += -I./include -I../include -I./libetherdream -I./liboptimizer/include
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
	cd liboptimizer/; $(MAKE)


.PHONY: etherdream
etherdream:
	cd libetherdream/; $(MAKE)


.PHONY: test
test: all
	$(CC) $(CFLAGS) $(LIBS) test/frame_server.c -o frame_server


.PHONY: install
install:
	# cd liboptimizer/; $(MAKE) install


.PHONY: clean
clean:
	cd libetherdream/; $(MAKE) clean
	cd liboptimizer/; $(MAKE) clean
	rm -f *.o lzrd frame_server
