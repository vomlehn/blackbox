/*
 * Functions and data for the RAM-based black box device
 */

#include <linux/err.h>
#include <linux/errno.h>
#include <linux/module.h>

#include <uapi/linux/blackbox-ram.h>

struct blackbox_ram {
	struct blackbox	blackbox;
};

static struct blackbox_ram blackbox_ram;

static struct blackbox_buf *bbram_buf_alloc(struct blackbox_device *bb_dev, off_t off)
{
	return ERR_PTR(-ENOSYS);
}

static ssize_t bbram_buf_write(struct blackbox_buf *bb_buf, off_t off, void *data,
	size_t size)
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

void blackbox_ramdev_init(struct blackbox_ram_device *bbram_dev)
{
	blackbox_device_init(&bbram_dev->blackbox_device);
	CLEAR_CHILD_SPECIFIC(bbram_dev, blackbox_device);
}

/*
 * Register the class of RAM-based blackbox devices
 */
static int blackbox_ram_module_init(void)
{
	return blackbox_init(&blackbox_ram.blackbox, "blackbox-ram", &bbram_devops);
}

static void blackbox_ram_module_exit(void)
{
	blackbox_fini(&blackbox_ram.blackbox);
}

module_init(blackbox_ram_module_init);
module_exit(blackbox_ram_module_exit);
MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("David VomLehn");
