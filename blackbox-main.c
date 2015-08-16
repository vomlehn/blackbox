#include <linux/kernel.h>
#include <linux/module.h>
#include <uapi/linux/blackbox.h>

static int blackbox_init(void)
{
	return 0;
}

static void blackbox_exit(void)
{
	pr_info("So long to blackbox\n");
}

module_init(blackbox_init);
module_exit(blackbox_exit);
MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("David VomLehn");
