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

/* Forward declaration */
struct blackbox_device;

/*
 * struct blackbox_devops - black box device operations
 * @bbdev_alloc:	Pointer to function that allocates a buffer:
 *			@bb_dev:	Pointer to the &struct blackbox_device to which
 *					the allocated buffer will belong.
 *			@off:		Offset within the device for which the buffer to
 *					be allocated
 *			Returns an ERR_PTR. If not IS_ERR(), this is a pointer to
 *			the allocated &struct blackbox_buf.
 * @bbdev_write:	Writes data to the previously allocated &struct blackbox_buf.
 *			@bb_buf:	Pointer to the &struct blackbox_buf in which
 *					data is to be stored
 * FIXME: should this be subsumed in the bbdev() call?
 *			@off:		Offset within the device at which to store
 *					data.
 *			@data:		Pointer to the data to write.
 *			@size:		Number of bytes to write
 *			Returns the number of bytes written if >= zero, otherwise
 *			a negative errno value.
 * @bbdev_commit	Writes the given &struct blackbox_buf to the device.
 *			@bb_buf:	Pointer to the &struct blackbox_buf to be
 *					commited.
 *			Returns zero on success or a negative errno value on failure.
 */
struct blackbox_devops {
	struct blackbox_buf	*(*bbdev_buf_alloc)(struct blackbox_device *bb_dev,
					off_t off);
	ssize_t			(*bbdev_buf_write)(struct blackbox_buf *bb_buf,
					off_t off, void *data, size_t size);
	int			(*bbdev_buf_commit)(struct blackbox_buf *bb_buf);
};

/*
 * Information about a class of blackbox devices
 */
struct blackbox {
	struct list_head		registered;
	const char			*name;
	const struct blackbox_devops	*ops;
};

/*
 * struct blackbox_header - header for write blocks. There may be more than one of these
 *	in a write block.
 */
struct blackbox_header {
};

/*
 * struct blackbox_buf - common data for a black box buffer
 * @bbdev:	Pointer to the &struct blackbox_device for the device that owns this
 *		buffer.
 */
struct blackbox_buf {
	struct blackbox_device	*bb_dev;
};

/*
 * struct blackbox_device - information about a black box device
 */
struct blackbox_device {
	struct blackbox	*blackbox;
};

/* Functions for the parent class of all blackbox device class classes */
extern int blackbox_init(struct blackbox *blackbox, const char *name,
	const struct blackbox_devops *devops);
extern void blackbox_fini(struct blackbox *blackbox);

/* Functions for individual blackbox devices */
extern void blackbox_device_init(struct blackbox_device *blackbox_device);
extern void blackbox_device_fini(struct blackbox_device *blackbox_device);
extern void blackbox_device_register(struct blackbox_device *blackbox_device);
extern void blackbox_device_unregister(struct blackbox_device *blackbox_device);

static inline struct blackbox_buf *
blockbox_buf_alloc(struct blackbox_device *blackbox_device, off_t off)
{
	return blackbox_device->blackbox->ops->bbdev_buf_alloc(blackbox_device, off);
}

static inline ssize_t blackbox_buf_write(struct blackbox_buf *blackbox_buf,
	off_t off, void *data, size_t size)
{
	return blackbox_buf->bb_dev->blackbox->ops->bbdev_buf_write(blackbox_buf, off,
		data, size);
}

static inline int blackbox_buf_commit(struct blackbox_buf *blackbox_buf)
{
	return blackbox_buf->bb_dev->blackbox->ops->bbdev_buf_commit(blackbox_buf);
}
#endif /* _LINUX_BLACKBOX_H_ */
