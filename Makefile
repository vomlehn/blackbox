KDIR		?= /usr/src/linux-headers-$(shell uname -r)
EXTRA_CFLAGS	+= -Wall -Werror -Wextra -Wno-unused-parameter
EXTRA_CFLAGS	+= -I$(PWD)/include
CROSS_COMPILE	=
ARCH			= x86_64
V			=  0
release			= $(shell uname -r)
module_dir		= /lib/modules/$(release)/kernel/drivers/misc
conf_dir		= /lib/modprobe.d

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

$(module_dir)/blackbox.ko: blackbox/blackbox.ko
	sudo cp $^ $@

$(module_dir)/blackbox-ram.ko: blackbox-ram/blackbox-ram.ko
	sudo cp $^ $@

$(conf_dir)/blackbox.conf: blackbox/blackbox.conf
	sudo cp $^ $@
	sudo depmod

$(conf_dir)/blackbox-ram.conf: blackbox-ram/blackbox-ram.conf
	sudo cp $^ $@
	sudo depmod

test: default \
	$(conf_dir)/blackbox.conf $(conf_dir)/blackbox-ram.conf \
	$(module_dir)/blackbox.ko $(module_dir)/blackbox-ram.ko
	./test.sh

clean:
	$(MAKE_CMD) clean

.PHONY:	clean
