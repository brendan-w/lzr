


# compile settings

CC      = gcc
CFLAGS += -std=c99 -Wall -Wextra -pedantic -fPIC
CFLAGS += -I.
LIBS    = -lzmq -lm
ARFLAGS = rs


# files and paths

# PREFIX ?= /usr/local

OBJS += interpolator/lzr_interpolator.o

OBJS += optimizer/lzr_optimizer.o \
        optimizer/find_paths.o \
        optimizer/rearrange_paths.o \
        optimizer/compile_paths.o

OBJS += zmq/lzr_zmq.o



# targets

.PHONY: all
all: liblzr.so liblzr.a


liblzr.so: $(OBJS)
	$(CC) -shared $^ $(LIBS) -o $@


liblzr.a: $(OBJS)
	$(AR) $(ARFLAGS) $@ $^


%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@






# .PHONY: test
# test: all test/frame_server.c
# 	$(CC) $(CFLAGS) $(LIBS) -lm test/frame_server.c -o frame_server


.PHONY: install
install:
	# $(MAKE) -C liblzr/ install


.PHONY: clean
clean:
	rm -f *.a *.o *.so
	rm -f interpolator/*.o optimizer/*.o zmq/*.o
