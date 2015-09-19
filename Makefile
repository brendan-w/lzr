

.PHONY: all
all: liblzr lzrd lzrv


.PHONY: liblzr
liblzr:
	$(MAKE) -C liblzr/


.PHONY: lzrd
lzrd:
	$(MAKE) -C lzrd/


.PHONY: lzrv
lzrv:
	$(MAKE) -C lzrv/


.PHONY: install
install:
	# $(MAKE) -C lzrd/ install


.PHONY: clean
clean:
	$(MAKE) -C liblzr/ clean
	$(MAKE) -C lzrd/ clean
	$(MAKE) -C lzrv/ clean
