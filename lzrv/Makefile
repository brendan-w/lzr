

# ============================= compile settings =============================


CC      = gcc
CFLAGS += -std=gnu99 -Wall -Wextra -pedantic -ggdb
CFLAGS += -I../liblzr -I/usr/include/SDL2
LIBS    = -lzmq -lm -lpthread -lSDL2


# ============================= files and paths =============================


INSTALL_PREFIX ?= /usr/local

OBJS += lzrv.o

HEADERS += ../liblzr/lzr.h lzrv.h

# ============================= main targets =============================


.PHONY: all
all: lzrv


lzrv: $(OBJS) ../liblzr/liblzr.a
	$(CC) $^ ../liblzr/liblzr.a $(LIBS) -o $@


%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@


.PHONY: install
install:
	# TODO


.PHONY: clean
clean:
	rm -f *.o lzrv
