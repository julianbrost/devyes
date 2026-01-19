# SPDX-FileCopyrightText: 2020 Julian Brost <julian@0x4a42.net>
# SPDX-License-Identifier: GPL-2.0-or-later

KDIR ?= /lib/modules/$(shell uname -r)/build

.PHONY: all
all:
	$(MAKE) -C $(KDIR) $(MAKE_OPTS) M=$(CURDIR) modules

.PHONY: clean
clean:
	$(MAKE) -C $(KDIR) $(MAKE_OPTS) M=$(CURDIR) clean
