

# ============================= compile settings =============================


CC      = gcc
CFLAGS += -std=gnu99 -Wall -Wextra -pedantic -fPIC -ggdb
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

OBJS += transforms/frame_transforms.o \
        transforms/point_transforms.o

OBJS += ilda/lzr_ilda.o \
        ilda/ilda_read.o \
        ilda/ilda_write.o


HEADERS += lzr.h \
           ilda/lzr_ilda.h \
           optimizer/lzr_optimizer.h

# ============================= main targets =============================


.PHONY: all
all: liblzr.so liblzr.a


liblzr.so: $(OBJS)
	$(CC) -shared $^ $(LIBS) -o $@


liblzr.a: $(OBJS)
	$(AR) $(ARFLAGS) $@ $^


%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@


.PHONY: install
install:
	install -m 0755 liblzr.a  $(INSTALL_PREFIX)/lib
	install -m 0755 liblzr.so $(INSTALL_PREFIX)/lib
	install -m 0755 lzr.h     $(INSTALL_PREFIX)/include


.PHONY: clean
clean:
	rm -f *.a *.o *.so
	rm -f interpolator/*.o
	rm -f transforms/*.o
	rm -f optimizer/*.o
	rm -f ilda/*.o
	rm -f zmq/*.o
	rm -f test_optimizer test_interpolator test_ilda_read test_ilda_write test_transformations


# ============================= test targets =============================


.PHONY: test
test: test_interpolator test_optimizer test_ilda_read test_ilda_write test_transformations


test_optimizer: all tests/test_optimizer.c
	$(CC) $(CFLAGS) $(LIBS) tests/test_optimizer.c ./liblzr.a -o test_optimizer


test_interpolator: all tests/test_interpolator.c
	$(CC) $(CFLAGS) $(LIBS) tests/test_interpolator.c ./liblzr.a -o test_interpolator


test_ilda_read: all tests/test_ilda_read.c
	$(CC) $(CFLAGS) $(LIBS) tests/test_ilda_read.c ./liblzr.a -o test_ilda_read


test_ilda_write: all tests/test_ilda_write.c
	$(CC) $(CFLAGS) $(LIBS) tests/test_ilda_write.c ./liblzr.a -o test_ilda_write


test_transformations: all tests/test_transformations.c
	$(CC) $(CFLAGS) $(LIBS) tests/test_transformations.c ./liblzr.a -o test_transformations
