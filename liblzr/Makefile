# ============================= compile settings =============================

CXX       = g++
CXXFLAGS += -std=c++11 -Wall -Wextra -pedantic -fPIC -ggdb
CXXFLAGS += -I.
LIBS      = -lm -lpthread
ARFLAGS   = rs

# ============================= files and paths =============================

INSTALL_PREFIX ?= /usr/local

SRC += $(wildcard src/*.cpp)
SRC += $(wildcard src/dac/*.cpp)
OBJS = $(SRC:.cpp=.o)

HEADERS := liblzr.hpp \
		   engine.hpp \
           src/*.hpp \
           src/dac/*.hpp

DACS := src/dac/libetherdream/etherdream.a


TOOLS_SRC = $(wildcard tools/*.cpp)
TOOLS = $(TOOLS_SRC:.cpp=)


TESTS_SRC = $(wildcard tests/*.cpp)
TESTS = $(TESTS_SRC:.cpp=)

# ============================= main targets =============================

.PHONY: all
all: liblzr.so liblzr.a $(TOOLS)

.PHONY: test
test: $(TESTS)

liblzr.so: $(OBJS) $(DACS)
	$(CXX) -shared $^ $(LIBS) -o $@

liblzr.a: $(OBJS)
	$(AR) $(ARFLAGS) $@ $^

%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

tests/%: tests/%.cpp all
	$(CXX) $(CXXFLAGS) $(LIBS) $< ./liblzr.a -o $@

tools/ilda-view: tools/ilda-view.cpp
	$(CXX) $(CXXFLAGS) -I/usr/include/SDL2 -lSDL2 $< ./liblzr.a -o $@

# DAC specific compilation
src/dac/libetherdream/etherdream.a:
	$(MAKE) -C $(dir $@)

.PHONY: install
install:
	install -m 0755 liblzr.a   $(INSTALL_PREFIX)/lib
	install -m 0755 liblzr.so  $(INSTALL_PREFIX)/lib
	install -m 0755 liblzr.hpp $(INSTALL_PREFIX)/include

.PHONY: clean
clean:
	rm -f *.so *.a
	find ./ -name "*.o" -type f -delete
	find ./ -executable -type f -delete
	$(foreach dac,$(DACS),$(MAKE) -C $(dir $(dac)) clean)
