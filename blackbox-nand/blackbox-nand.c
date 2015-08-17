/*
 * Functions and data for the NAND flash-based black box device
 */

#include <linux/err.h>
#include <linux/errno.h>
#include <linux/fs.h>
#include <linux/module.h>

#include <uapi/linux/blackbox-nand.h>

struct blackbox_nand {
	struct blackbox	blackbox;
};

static struct blackbox_nand blackbox_nand;

static struct blackbox_buf *bbnand_buf_alloc(struct blackbox_dev *bb_dev,
	off_t off)
{
	return ERR_PTR(-ENOSYS);
}

static ssize_t bbnand_buf_write(struct blackbox_buf *bb_buf, void *data,
	size_t size, off_t off)
{
	return -ENOSYS;
}

static int bbnand_buf_commit(struct blackbox_buf *bb_buf)
{
	return -ENOSYS;
}

static const struct blackbox_devops bbnand_devops = {
	.bbdev_buf_alloc = bbnand_buf_alloc,
	.bbdev_buf_write = bbnand_buf_write,
	.bbdev_buf_commit = bbnand_buf_commit,
};

/*
 * Initialize a NAND-based blackbox device
 */
void blackbox_nanddev_init(struct blackbox_dev_nand *bbnand_dev)
{
	blackbox_dev_init(&bbnand_dev->blackbox_dev);
	CLEAR_CHILD_SPECIFIC(bbnand_dev, blackbox_dev);
}

/*
 * Register the class of NAND-based blackbox devices
 */
static int blackbox_nand_module_init(void)
{
	return blackbox_init(&blackbox_nand.blackbox, "blackbox-nand", &bbnand_devops);
}

static void blackbox_nand_module_exit(void)
{
	blackbox_fini(&blackbox_nand.blackbox);
}

module_init(blackbox_nand_module_init);
module_exit(blackbox_nand_module_exit);
MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("David VomLehn");
