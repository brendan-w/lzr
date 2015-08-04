

# files and paths

VPATH = src
# PREFIX ?= /usr/local
OBJS  = lzrd.o


# compile settings

CC      = gcc
CFLAGS += -std=c99 -Wall -Wextra -pedantic -ggdb
CFLAGS += -I../liblzr
LIBS    = -lzmq -lpthread


# targets

.PHONY: all
all: lzrd


lzrd: ../liblzr/liblzr.a etherdream $(OBJS)
	$(CC) $(OBJS) ./libetherdream/etherdream.a ../liblzr/liblzr.a $(LIBS) -o $@


%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)


../liblzr/liblzr.a:
	$(MAKE) -C ../liblzr/


.PHONY: etherdream
etherdream:
	$(MAKE) -C libetherdream/


.PHONY: test
test: all test/frame_server.c
	$(CC) $(CFLAGS) $(LIBS) -lm test/frame_server.c -o frame_server


.PHONY: install
install:
	# $(MAKE) -C liblzr_optimize/ install


.PHONY: clean
clean:
	$(MAKE) -C libetherdream/ clean
	rm -f *.o lzrd frame_server
