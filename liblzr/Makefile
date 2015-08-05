

# ============================= compile settings =============================


CC      = gcc
CFLAGS += -std=c99 -Wall -Wextra -pedantic -fPIC
CFLAGS += -I.
LIBS    = -lzmq -lm
ARFLAGS = rs


# ============================= files and paths =============================


INSTALL_PREFIX ?= /usr/local

OBJS += interpolator/lzr_interpolator.o

OBJS += optimizer/lzr_optimizer.o \
        optimizer/find_paths.o \
        optimizer/rearrange_paths.o \
        optimizer/compile_paths.o

OBJS += zmq/lzr_zmq.o


# ============================= main targets =============================


.PHONY: all
all: liblzr.so liblzr.a


liblzr.so: $(OBJS)
	$(CC) -shared $^ $(LIBS) -o $@


liblzr.a: $(OBJS)
	$(AR) $(ARFLAGS) $@ $^


%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@


.PHONY: install
install:
	install -m 0755 liblzr.a  $(INSTALL_PREFIX)/lib
	install -m 0755 liblzr.so $(INSTALL_PREFIX)/lib
	install -m 0755 lzr.h     $(INSTALL_PREFIX)/include


.PHONY: clean
clean:
	rm -f *.a *.o *.so
	rm -f interpolator/*.o optimizer/*.o zmq/*.o
	rm -f test_optimizer test_interpolator


# ============================= test targets =============================


.PHONY: test
test: test_interpolator test_optimizer


test_optimizer: all tests/test_optimizer.c
	$(CC) $(CFLAGS) $(LIBS) tests/test_optimizer.c ./liblzr.a -o test_optimizer
	./test_optimizer


test_interpolator: all tests/test_interpolator.c
	$(CC) $(CFLAGS) $(LIBS) tests/test_interpolator.c ./liblzr.a -o test_interpolator
	./test_interpolator
