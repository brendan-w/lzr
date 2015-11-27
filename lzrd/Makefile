

# ============================= compile settings =============================


CXX       = g++
CXXFLAGS += -std=c++11 -Wall -Wextra -pedantic -fPIC -ggdb
CXXFLAGS += -I../liblzr
LIBS      = -lzmq -lm -lpthread


# ============================= files and paths =============================


INSTALL_PREFIX ?= /usr/local

OBJS := lzrd.o


HEADERS += lzr.h

TESTS := spiro_server \
         interp_server \
         ilda_server \
         transform_server


# ============================= main targets =============================


.PHONY: all
all: lzrd


lzrd: $(OBJS) ../liblzr/liblzr.a libetherdream/etherdream.a
	$(CXX) $^ $(LIBS) -o $@


%.o: %.c
	$(CXX) -c -o $@ $< $(CXXFLAGS)


../liblzr/liblzr.a:
	$(MAKE) -C ../liblzr/


libetherdream/etherdream.a:
	$(MAKE) -C libetherdream/


.PHONY: test
test: all test/*.c
	for test in $(TESTS); do \
		$(CXX) $(CXXFLAGS) $(LIBS) -lm test/$$test.c ../liblzr/liblzr.a -o $$test ; \
	done


.PHONY: clean
clean:
	$(MAKE) -C libetherdream/ clean
	rm -f *.o lzrd $(TESTS)
