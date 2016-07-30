
.PHONY: all
all: liblzr lzrv

.PHONY: liblzr
liblzr:
	$(MAKE) -C liblzr/

.PHONY: lzrv
lzrv:
	$(MAKE) -C lzrv/

.PHONY: test
test:
	$(MAKE) -C liblzr/ test

.PHONY: clean
clean:
	$(MAKE) -C liblzr/ clean
	$(MAKE) -C lzrv/ clean
