KDIR		?= /usr/src/linux-headers-$(shell uname -r)
EXTRA_CFLAGS	+= -Wall -Werror -Wextra -Wno-unused-parameter
EXTRA_CFLAGS	+= -I$(PWD)/include
CROSS_COMPILE	=
ARCH		= x86_64
V		=  0

MAKE_CMD	= $(MAKE) -C $(KDIR) \
			V=$(V) \
			M=$$PWD \
			ARCH=$(ARCH) \
			CROSS_COMPILE=$(CROSS_COMPILE) \
			EXTRA_CFLAGS="$(EXTRA_CFLAGS)"

default:
	$(MAKE_CMD)

install: default

all_objects=$($(mod)-y) built-in.o $(mod).o $(mod).mod.o

test: default
	@echo Not testing yet
	@echo But I would do an insmod
	exit 1

clean:
	$(MAKE_CMD) clean

.PHONY:	clean
