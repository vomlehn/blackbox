KDIR		?= /usr/src/linux-headers-3.19.0-25-generic
EXTRA_CFLAGS	+= -Wall -Werror -Wextra -Wno-unused-parameter
EXTRA_CFLAGS	+= -I$(PWD)/include
CROSS_COMPILE	=
ARCH		= x86_64
MAKE_CMD	= $(MAKE) -C $(KDIR) \
			M=$$PWD \
			ARCH=$(ARCH) \
			CROSS_COMPILE=$(CROSS_COMPILE) \
			EXTRA_CFLAGS="$(EXTRA_CFLAGS)"

default:
	$(MAKE_CMD)

install: default

all_objects=$($(mod)-y) built-in.o $(mod).o $(mod).mod.o

clean:
	$(MAKE_CMD) clean

.PHONY:	clean
