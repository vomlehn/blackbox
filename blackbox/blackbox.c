/*
 * Functions common to all blackbox devices.
 */

#include <linux/kernel.h>
#include <linux/module.h>

#include <uapi/linux/blackbox.h>

static LIST_HEAD(registered);

int blackbox_init(struct blackbox *blackbox, const char *name,
	const struct blackbox_devops *devops)
{
	blackbox->ops = devops;
	blackbox->name = name;
	list_add(&blackbox->registered, &registered);

	return 0;
}

void blackbox_fini(struct blackbox *blackbox)
{
	list_del(&blackbox->registered);
}

int blackbox_module_init(void)
{
	return 0;
}

static void blackbox_module_exit(void)
{
	pr_info("So long to blackbox\n");
}

module_init(blackbox_module_init);
module_exit(blackbox_module_exit);
MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("David VomLehn");
