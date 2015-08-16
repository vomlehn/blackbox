#include Kbuild
mod := blackbox
$(mod)-y	:= blackbox-main.o blackbox-ram.o blackbox-nand.o

KDIR		?= /usr/src/linux-headers-3.19.0-25-generic
EXTRA_CFLAGS	+= -Wall -Werror -Wextra -Wno-unused-parameter
EXTRA_CFLAGS	+= -I$(PWD)/include
CROSS_COMPILE	=
ARCH		= x86_64

default:
	$(MAKE) -C $(KDIR) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) \
		M=$$PWD EXTRA_CFLAGS="$(EXTRA_CFLAGS)"

install: default
	adb push $(mod).ko /data
	adb shell insmod /data/$(mod).ko
	adb shell "dmesg | busybox tail -n 40"
	adb shell rmmod $(mod)

all_objects=$($(mod)-y) built-in.o $(mod).o $(mod).mod.o

clean:
	rm -f $(all_objects)
	rm -f $(patsubst %.o,.%.o.cmd,$(all_objects))
	rm -f $(mod).ko .$(mod).ko.cmd $(mod).mod.c modules.order Module.symvers
	rm -rf .tmp_versions

.PHONY:	clean
