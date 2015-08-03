

# files and paths

OBJS  = lzr_interpolator.o


# compile settings

CC=gcc
CFLAGS += -std=c99 -Wall -Wextra -pedantic -fPIC
CFLAGS += -I../../include
LIBS=-lm

ARFLAGS=rvs


# targets

.PHONY: all
all: lzr_interpolator.a


lzr_interpolator.a: $(OBJS)
	$(AR) $(ARFLAGS) $@ $^


%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)


.PHONY: test
test: all test.c
	$(CC) $(CFLAGS) $(LIBS) test.c ./lzr_interpolator.a -o test_interpolator
	./test_interpolator


.PHONY: clean
clean:
	rm -f *.a *.o *.so test_interpolator
