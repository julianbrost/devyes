KDIR ?= /lib/modules/$(shell uname -r)/build

.PHONY: all
all:
	$(MAKE) -C $(KDIR) $(MAKE_OPTS) M=$(CURDIR) modules

.PHONY: clean
clean:
	$(MAKE) -C $(KDIR) $(MAKE_OPTS) M=$(CURDIR) clean
