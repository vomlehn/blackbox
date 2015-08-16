/*
 * Definitions for NAND-based black box devices
 */

#ifndef _LINUX_BLACKBOX_NAND_H_
#define _LINUX_BLACKBOX_NAND_H_

#include <uapi/linux/blackbox.h>

/*
 * struct blackbox_nand_desc - descriptor for one piece of a NAND-based black box device
 * @addr:	Virtual address of this piece of the device
 * @size:	Number of bytes in this piece of the device
 */
struct blackbox_nand_desc {
	void	*addr;
	size_t	size;
};

/*
 * struct blackbox_nand_buf - buffer for NAND-based black box device
 * @blackbox_buf:	Generic black box buffer information
 */
struct blackbox_nand_buf {
	struct blackbox_buf		blackbox_buf;
};

/*
 * struct blackbox_nand_device - information about a NAND-based blackbox device
 * @blackbox_device:	Generic black box information
 * @n_desc:		Number of &struct blackbox_nand_desc structures used to describe
 *			the memory occupied by this device
 * @desc:		Descriptors used to describe the memory occupied by this device
 *			in order of increasing offset within the device
 */
struct blackbox_nand_device {
	struct blackbox_device		blackbox_device;
	size_t				n_desc;
	struct blackbox_nand_desc	desc[];
};

#endif /* _LINUX_BLACKBOX_NAND_H_ */
