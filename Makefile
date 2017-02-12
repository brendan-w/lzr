
.PHONY: all
all: liblzr

.PHONY: liblzr
liblzr:
	$(MAKE) -C liblzr/

.PHONY: test
test:
	$(MAKE) -C liblzr/ test

.PHONY: clean
clean:
	$(MAKE) -C liblzr/ clean
