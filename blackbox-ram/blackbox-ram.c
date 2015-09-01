/*
 * Functions and data for the RAM-based black box device
 */

#include <linux/err.h>
#include <linux/errno.h>
#include <linux/fs.h>
#include <linux/module.h>
#include <linux/init.h>

#include <linux/blackbox-ram.h>

static char *params;

/*
 * This is the dummy RAM area for development it needs to be replaced by
 * passed-in parameters.
 */
unsigned char dummy_ram[1024*1024];
size_t dummy_ram_sixe = sizeof(dummy_ram);

/*
 * struct blackbox_ram_desc - descriptor for one piece of a RAM-based black box
 *	device
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
 * struct blackbox_dev_ram - information about a RAM-based blackbox device
 * @blackbox_dev:	Generic black box information
 * @n_desc:		Number of &struct blackbox_ram_desc structures used to
 *			describe the memory occupied by this device
 * @desc:		Descriptors used to describe the memory occupied by
 *			this device in order of increasing offset within the
 *			device
 */
struct blackbox_dev_ram {
	struct blackbox_device		blackbox_device;
	size_t				n_desc;
	struct blackbox_ram_desc	desc[];
};

struct blackbox_ramdev {
	struct blackbox_device	blackbox_device;
};

struct blackbox_ram {
	struct blackbox_class	blackbox_class;
};

static struct blackbox_ram blackbox_ram;
static bool blackbox_ram_initialized;
static bool initialized;

/*
 * bbram_buf_alloc - allocate a buffer for writing
 * @bb_dev:	Pointer to the parent &struct blackbox_dev
 * @off:	Offset within the blackbox device for which the buffer is
 *		to be allocated
 *
 * On success, returns a pointer to a &struct blackbox_buf, otherwise an
 * ERR_PTR.
 */
static struct blackbox_buf *bbram_buf_alloc(struct blackbox_device *bb_dev,
	off_t off)
{
	struct blackbox_ramdev *ramdev;

	ramdev = container_of(bb_dev, struct blackbox_ramdev, blackbox_device);

	return ERR_PTR(-ENOSYS);
}

/*
 * bbram_buf_write - write bytes to a &struct blackbox_buf
 * @bb_buf:	Pointer to the &struct blackbox_buf to which to write data
 * @data:	Pointer to the data to write
 * @size:	Number of bytes to write
 * @off:	Offset in bytes from the start of the device
 */
static ssize_t bbram_buf_write(struct blackbox_buf *bb_buf, void *data,
	size_t size, off_t off)
{
	return -ENOSYS;
}

static int bbram_buf_commit(struct blackbox_buf *bb_buf)
{
	return -ENOSYS;
}

static const struct blackbox_devops bbram_devops = {
	.bbdev_buf_alloc = bbram_buf_alloc,
	.bbdev_buf_write = bbram_buf_write,
	.bbdev_buf_commit = bbram_buf_commit,
};

static ssize_t bbram_read(struct file *f, char __user *buf, size_t count,
	loff_t *off)
{
	return -ENOSYS;
}

static ssize_t bbram_write(struct file *f, const char __user *buf, size_t count,
	loff_t *off)
{
	return -ENOSYS;
}

static int bbram_open(struct inode *inode, struct file *f)
{
	return -ENOSYS;
}

static int bbram_release(struct inode *inode, struct file *f)
{
	return -ENOSYS;
}

static const struct file_operations fops = {
	.owner = THIS_MODULE,
	.read = bbram_read,
	.write = bbram_write,
	.open = bbram_open,
	.release = bbram_release,
};

/*
 * blackbox_ramdev_init - initialize a RAM-based blackbox device
 * @bbram_dev:	Pointer to a &struct blackbox_dev_ram
 * @ram_addr:	Pointer to the start of RAM to be used for the device
 * @size:	Number of bytes in the device
 */
void blackbox_ramdev_init(struct blackbox_dev_ram *bbram_dev, void *ram,
	size_t size)
{
	blackbox_device_init(&bbram_dev->blackbox_device);
	CLEAR_CHILD_SPECIFIC(bbram_dev, blackbox_device);
}

#ifndef MODULE
static int __init setup_early_blackbox(char *p)
{
	char *retptr;
	unsigned long long size, start;

printk("%s: given %s\n", __func__, p);
return 0;
	size = memparse(p, &retptr);
	if (*retptr != '@')
		return -EINVAL;

	start = memparse(retptr + 1, &retptr);

	if (*retptr != ' ' && *retptr != '\0')
		return -EINVAL;

	// FIXME: needs work, may be in the wrong place.
	blackbox_add_device("ram", size, start);

	return 0;
}
#endif

#ifndef MODULE
early_param(blackbox_ram, setup_early_blackbox);
#endif

/*
 * Register the class of RAM-based blackbox devices.
 */
static int blackbox_ram_module_init(void)
{
	int rc;
	pr_info("Wave hello to %s\n", __func__);
	pr_info("params = %s\n", params);

	/*
	 * This may be called explicitly if early use of the RAM-based Blackblox
	 * driver is configured but is always called during normal driver
	 * initialization. Make sure we do the initialization work once.
	 */
	if (blackbox_ram_initialized)
		return 0;

	blackbox_ram_initialized = true;

	/*
	 * Ensure the underlying common Blackbox code is initialized so that
	 * we work even in the case of early initialization
	 */
	rc = blackbox_module_init();
	if (rc != 0)
		return 0;

	blackbox_class_register(&blackbox_ram.blackbox_class, "blackbox-ram",
		&bbram_devops);

	return 0;
}

static void blackbox_ram_module_exit(void)
{
	pr_info("Wave goodbye to %s\n", __func__);
	blackbox_class_unregister(&blackbox_ram.blackbox_class);
	initialized = false;
}

module_param(params, charp, 0644);
MODULE_PARM_DESC(params, "parameters: size@physical_address");

module_init(blackbox_ram_module_init);
module_exit(blackbox_ram_module_exit);
MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("David VomLehn");
