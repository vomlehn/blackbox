/*
 * Definitions for RAM-based black box devices
 */

#ifndef _LINUX_BLACKBOX_RAM_H_
#define _LINUX_BLACKBOX_RAM_H_

#include <uapi/linux/blackbox.h>

/*
 * struct blackbox_ram_desc - descriptor for one piece of a RAM-based black box device
 * @addr:	Virtual address of this piece of the device
 * @size:	Number of bytes in this piece of the device
 */
struct blackbox_ram_desc {
	void	*addr;
	size_t	size;
};

/*
 * struct blackbox_ram_buf - buffer for RAM-based black box device
 * @blackbox_buf:	Generic black box buffer information
 */
struct blackbox_ram_buf {
	struct blackbox_buf		blackbox_buf;
};

/*
 * struct blackbox_ram_device - information about a RAM-based blackbox device
 * @blackbox_device:	Generic black box information
 * @n_desc:		Number of &struct blackbox_ram_desc structures used to describe
 *			the memory occupied by this device
 * @desc:		Descriptors used to describe the memory occupied by this device
 *			in order of increasing offset within the device
 */
struct blackbox_ram_device {
	struct blackbox_device		blackbox_device;
	size_t				n_desc;
	struct blackbox_ram_desc	desc[];
};

#endif /* _LINUX_BLACKBOX_RAM_H_ */
