/*
 * Definitions for the blackbox device
 */

#ifndef _LINUX_BLACKBOX_H_
#define _LINUX_BLACKBOX_H_

#include <linux/kernel.h>
#include <linux/list.h>
#include <linux/string.h>

/*
 * Macro that zeros out the memory between the end of an embedded structure
 * (the child structure) and the end of tjhe structure in which it is embedded
 * (the parent structure). The child structure must be the first element in
 * the parent structure.
 *
 * @parentp:	Pointer to the parent structure
 * @child:	Name of the chaild structure
 */
#define CLEAR_CHILD_SPECIFIC(parent_p, child) \
	memset((void *)&(parent_p)->child + sizeof((parent_p)->child), 0, \
		sizeof(*(parent_p)) - sizeof((parent_p)->child))

/* Maximum length including trailing NUL character */
#define BLACKBOX_MAX_NAME_LEN	16

/*
 * struct blackbox_header - header for write blocks. There may be more than
 *	one of these in a write block.
 */
struct blackbox_header {
	unsigned char	seqno[16];
	unsigned char	version[2];
	unsigned char	offset[8];
	unsigned char	size[8];
	unsigned char	checksum[8];
	unsigned char	eoh[1];
};
#endif /* _LINUX_BLACKBOX_H_ */
