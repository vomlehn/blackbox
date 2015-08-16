/*
 * Functions and data for the RAM-based black box device
 */

#include <linux/err.h>
#include <linux/errno.h>

#include <uapi/linux/blackbox-ram.h>

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

static const struct blackbox_devops blackbox_devops = {
	.bbdev_buf_alloc = bbram_buf_alloc,
	.bbdev_buf_write = bbram_buf_write,
	.bbdev_buf_commit = bbram_buf_commit,
};
