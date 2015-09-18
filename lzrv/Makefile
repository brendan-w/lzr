

# ============================= compile settings =============================


CXX      = g++
CXXFLAGS += -Wall -Wextra -pedantic
CXXFLAGS += -I../liblzr -I/usr/include/SDL2
LIBS    = -lzmq -lm -lSDL2


# ============================= files and paths =============================


INSTALL_PREFIX ?= /usr/local

OBJS += lzrv.o

HEADERS += ../liblzr/lzr.h lzrv.h

# ============================= main targets =============================


.PHONY: all
all: lzrv


lzrv: $(OBJS)
	$(CXX) $^ ../liblzr/liblzr.a $(LIBS) -o $@


%.o: %.c $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@


.PHONY: install
install:
	# TODO


.PHONY: clean
clean:
	rm -f *.o lzrv
