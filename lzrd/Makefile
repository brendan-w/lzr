

# files and paths

VPATH = src
# PREFIX ?= /usr/local
OBJS = lzrd.o

TESTS = spiro_server \
        interp_server \
        ilda_server \
        transform_server



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
test: all test/*.c
	for test in $(TESTS); do \
		$(CC) $(CFLAGS) $(LIBS) -lm test/$$test.c ../liblzr/liblzr.a -o $$test ; \
	done


.PHONY: install
install:
	# $(MAKE) -C liblzr_optimize/ install


.PHONY: clean
clean:
	$(MAKE) -C libetherdream/ clean
	rm -f *.o lzrd $(TESTS)
