

.PHONY: all
all: lzrd


.PHONY: lzrd
lzrd:
	$(MAKE) -C lzrd/


.PHONY: install
install:
	$(MAKE) -C lzrd/ install


.PHONY: clean
clean:
	$(MAKE) -C lzrd/ clean
