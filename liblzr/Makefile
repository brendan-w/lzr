

# ============================= compile settings =============================


CXX       = g++
CXXFLAGS += -std=c++11 -Wall -Wextra -pedantic -fPIC -ggdb
CXXFLAGS += -I.
LIBS      = -lm -lpthread
ARFLAGS   = rs


# ============================= files and paths =============================


INSTALL_PREFIX ?= /usr/local

OBJS := src/point.o \
        src/frame.o \
        src/ilda.o \
        src/ilda_read.o \
        src/ilda_write.o \
        src/ilda_palette.o \
        src/transforms.o \
        src/mask.o \
        src/interpolator.o \
        src/optimizer.o \
        src/optimizer_path.o \
        src/optimizer_find_paths.o \
        src/optimizer_reorder_paths.o \
        src/optimizer_compile_paths.o \
        src/engine.o \
        src/dac/dac.o \
        src/dac/etherdream.o

HEADERS := liblzr.h \
		   engine.hpp \
           src/*.h \
           src/dac/*.h

DACS := src/dac/libetherdream/etherdream.a

TESTS := test_interpolator \
         test_optimizer
         # test_optimizer \
         # test_ilda_read \
         # test_ilda_write \
         # test_transformations

# ============================= main targets =============================


.PHONY: all
all: liblzr.so liblzr.a


liblzr.so: $(OBJS) $(DACS)
	$(CXX) -shared $^ $(LIBS) -o $@


liblzr.a: $(OBJS)
	$(AR) $(ARFLAGS) $@ $^


%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@


# DAC specific compilation
src/dac/libetherdream/etherdream.a:
	$(MAKE) -C $(dir $@)


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
	$(foreach dac,$(DACS),$(MAKE) -C $(dir $(dac)) clean)


simple_server: tests/simple_server.cpp
	$(CXX) $(CXXFLAGS) $(LIBS) $^ ./liblzr.a -o $@


.PHONY: test
test: all tests/*.cpp
	for test in $(TESTS); do \
		$(CXX) $(CXXFLAGS) $(LIBS) tests/$$test.cpp ./liblzr.a -o $$test ; \
	done
