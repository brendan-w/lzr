

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

TESTS = test_optimizer \
        test_interpolator \
        test_ilda_read \
        test_ilda_write \
        test_transformations

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
	rm -f $(TESTS)


.PHONY: test
test: all tests/*.c
	for test in $(TESTS); do \
		$(CC) $(CFLAGS) $(LIBS) tests/$$test.c ./liblzr.a -o $$test ; \
	done
