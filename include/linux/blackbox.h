/*
 * Kernel definitions for the Blackbox failure log driver
 */

#ifndef _BLACKBOX_H_
#define _BLACKBOX_H_

#include <uapi/linux/blackbox.h>

/* Maximum number of Blackbox devices available */
#define BLACKBOX_MAX_COUNT	4

#ifdef CONFIG_BLACKBOX_EARLY_INIT
extern void blackbox_early_init(void);
#else
static inline void blackbox_early_init(void)
{
}
#endif

/*
 * struct blackbox_device - information about a Blackbox device
 * @blackbox_class:	Pointer to the associated Blackbox class
 * @size:		Size of the Blackbox device, in bytes
 * @offset:		Offset of the Blackbox device from some device-specific
 *			location, in bytes
 */
struct blackbox_device {
	struct blackbox_class	*class;
	u64			size;
	u64			offset;
};

/*
 * struct blackbox_devops - black box device operations
 * @bbdev_alloc:	Pointer to function that allocates a buffer:
 *			@bb_dev:	Pointer to the &struct blackbox_dev
 *					to which the allocated buffer will
 *					belong.
 *			@off:		Offset within the device for which the
 *					buffer to be allocated
 *			Returns an ERR_PTR. If not IS_ERR(), this is a pointer
 *			to the allocated &struct blackbox_buf.
 * @bbdev_write:	Writes data to the previously allocated &struct
 *			blackbox_buf.
 *			@bb_buf:	Pointer to the &struct blackbox_buf in
 *					which data is to be stored
 * FIXME: should this be subsumed in the bbdev() call?
 *			@off:		Offset within the device at which to
 *					store data.
 *			@data:		Pointer to the data to write.
 *			@size:		Number of bytes to write
 *			Returns the number of bytes written if >= zero,
 *			otherwise a negative errno value.
 * @bbdev_commit	Writes the given &struct blackbox_buf to the device.
 *			@bb_buf:	Pointer to the &struct blackbox_buf to
 *					be commited.
 *			Returns zero on success or a negative errno value on
 *			failure.
 */
struct blackbox_devops {
	struct blackbox_buf	*(*bbdev_buf_alloc)
					(struct blackbox_device *bb_dev,
					off_t off);
	ssize_t			(*bbdev_buf_write)(struct blackbox_buf *bb_buf,
					void *data, size_t size, off_t off);
	int			(*bbdev_buf_commit)(struct blackbox_buf *bb_buf);
};

/*
 * struct blackbox_class - information on a class of Blackbox devices
 */
struct blackbox_class {
	struct list_head		registered;
	const char			*name;
	const struct blackbox_devops	*devops;
};

/*
 * struct blackbox_buf - common data for a black box buffer
 * @bbdev:	Pointer to the &struct blackbox_dev for the device that owns
 *		this buffer.
 */
struct blackbox_buf {
	struct blackbox_device	*bb_dev;
};

/* Module initialization functios */
extern int blackbox_module_init(void);
extern void blackbox_module_exit(void);

/* Functions for the parent class of all blackbox device class classes */
extern int blackbox_init(struct blackbox_class *bb_class, const char *name,
	const struct blackbox_devops *devops);
extern void blackbox_fini(struct blackbox_class *bb_class);
extern void blackbox_class_unregister(struct blackbox_class *bb_class);
extern int blackbox_class_register(struct blackbox_class *bb_class,
	const char *name, const struct blackbox_devops *devops);

/* Functions for individual blackbox devices */
extern void blackbox_device_init(struct blackbox_device *blackbox_dev);
extern void blackbox_device_fini(struct blackbox_device *blackbox_dev);
extern void blackbox_device_register(struct blackbox_device *blackbox_dev);
extern void blackbox_device_unregister(struct blackbox_device *blackbox_dev);
// Is this used?
extern void blackbox_add_device(char *name, unsigned long long size,
	unsigned long long offset);

static inline struct blackbox_buf *
blockbox_buf_alloc(struct blackbox_device *blackbox_dev, off_t off)
{
	return blackbox_dev->class->devops->
		bbdev_buf_alloc(blackbox_dev, off);
}

static inline ssize_t blackbox_buf_write(struct blackbox_buf *blackbox_buf,
	void *data, size_t size, off_t off)
{
	return blackbox_buf->bb_dev->class->devops->
		bbdev_buf_write(blackbox_buf, data, size, off);
}

static inline int blackbox_buf_commit(struct blackbox_buf *blackbox_buf)
{
	return blackbox_buf->bb_dev->class->devops->
		bbdev_buf_commit(blackbox_buf);
}
#endif /* _BLACKBOX_H_ */
