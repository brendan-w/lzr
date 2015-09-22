

# files and paths

VPATH = src
# PREFIX ?= /usr/local
OBJS  = lzrd.o


# compile settings

CC      = gcc
CFLAGS += -std=c99 -Wall -Wextra -pedantic -ggdb
CFLAGS += -I../liblzr
LIBS    = -lzmq -lm -lpthread


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
test: all test/spiro_server.c test/interp_server.c test/ilda_server.c test/transform_server.c
	$(CC) $(CFLAGS) $(LIBS) -lm test/spiro_server.c     ../liblzr/liblzr.a -o spiro_server
	$(CC) $(CFLAGS) $(LIBS) -lm test/interp_server.c    ../liblzr/liblzr.a -o interp_server
	$(CC) $(CFLAGS) $(LIBS) -lm test/ilda_server.c      ../liblzr/liblzr.a -o ilda_server
	$(CC) $(CFLAGS) $(LIBS) -lm test/transform_server.c ../liblzr/liblzr.a -o transform_server


.PHONY: install
install:
	# $(MAKE) -C liblzr_optimize/ install


.PHONY: clean
clean:
	$(MAKE) -C libetherdream/ clean
	rm -f *.o lzrd spiro_server interp_server ilda_server transform_server
