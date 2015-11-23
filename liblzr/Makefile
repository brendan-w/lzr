

# ============================= compile settings =============================


CXX       = g++
CXXFLAGS += -std=c++11 -Wall -Wextra -pedantic -fPIC -ggdb
CXXFLAGS += -I.
LIBS      = -lzmq -lm
ARFLAGS   = rs


# ============================= files and paths =============================


INSTALL_PREFIX ?= /usr/local

OBJS := src/point.o \
        src/frame.o \
        src/zmq.o \
        src/ilda.o \
        src/ilda_read.o
        # src/ilda_write.o \
        # src/ilda_utils.o \


HEADERS += lzr.h

TESTS = test_optimizer \
        test_interpolator \
        test_ilda_read \
        test_ilda_write \
        test_transformations

# ============================= main targets =============================


.PHONY: all
all: liblzr.so liblzr.a


liblzr.so: $(OBJS)
	$(CXX) -shared $^ $(LIBS) -o $@


liblzr.a: $(OBJS)
	$(AR) $(ARFLAGS) $@ $^


%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@


.PHONY: install
install:
	install -m 0755 liblzr.a  $(INSTALL_PREFIX)/lib
	install -m 0755 liblzr.so $(INSTALL_PREFIX)/lib
	install -m 0755 lzr.h     $(INSTALL_PREFIX)/include


.PHONY: clean
clean:
	rm -f *.a *.o *.so
	rm -f src/*.o
	rm -f interpolator/*.o
	rm -f transforms/*.o
	rm -f optimizer/*.o
	rm -f ilda/*.o
	rm -f zmq/*.o
	rm -f $(TESTS)


simple_server: tests/simple_server.cpp
	$(CXX) $(CXXFLAGS) $(LIBS) $^ ./liblzr.a -o $@


# .PHONY: test
# test: all tests/*.c
# 	for test in $(TESTS); do \
# 		$(CXX) $(CXXFLAGS) $(LIBS) tests/$$test.c ./liblzr.a -o $$test ; \
# 	done
