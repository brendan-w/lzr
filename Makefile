

.PHONY: all
all: lzrd


.PHONY: lzrd
lzrd:
	cd lzrd/; $(MAKE)


.PHONY: install
install:
	cd lzrd/; $(MAKE) install


.PHONY: clean
clean:
	cd lzrd/; $(MAKE) clean
