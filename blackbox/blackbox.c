/*
 * Functions common to all blackbox devices.
 *
 * Because we want Blackbox device to work very early in start up, we defer
 * memory allocation, even if this might mean some features aren't available
 * until the whole system is up.
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>

#include <linux/blackbox.h>

static LIST_HEAD(registered);
static bool initialized;

static struct blackbox_device blackbox_devices[BLACKBOX_MAX_COUNT];

/*
 * blackbox_class_register - register a new Blackbox class
 */
int blackbox_class_register(struct blackbox_class *bb_class,
	const char *name, const struct blackbox_devops *devops)
{
	bb_class->devops = devops;
	bb_class->name = name;
	list_add(&bb_class->registered, &registered);

	return 0;
}
EXPORT_SYMBOL(blackbox_class_register);

void blackbox_class_unregister(struct blackbox_class *bb_class)
{
	list_del(&bb_class->registered);
}

static struct blackbox_class *find_bb_class(const char *name)
{
	struct blackbox_class *bb_class;

	list_for_each_entry(bb_class, &registered, registered) {
		if (strcmp(name, bb_class->name) == 0)
			return bb_class;
	}

	return NULL;
}
EXPORT_SYMBOL(blackbox_class_unregister);

void blackbox_add_device(char *name, unsigned long long size,
	unsigned long long offset)
{
	struct blackbox_class *bb_class;
	int i;

	bb_class = find_bb_class(name);
	if (bb_class == NULL) {
		pr_err("No blackbox class found with name %s\n", name);
		return;
	}

	for (i = 0; i < ARRAY_SIZE(blackbox_devices); i++) {
		if (blackbox_devices[i].class == NULL) {
			blackbox_devices[i].class = bb_class;
			blackbox_devices[i].size = size;
			blackbox_devices[i].offset = offset;
			return;
		}
	}

	pr_err("Too many blackbox devices defined\n");
}
EXPORT_SYMBOL(blackbox_add_device);

void blackbox_device_init(struct blackbox_device *dev)
{
	memset(dev, 0, sizeof(*dev));
}
EXPORT_SYMBOL(blackbox_device_init);

void blackbox_device_fini(struct blackbox_device *dev)
{
}
EXPORT_SYMBOL(blackbox_device_fini);

/*
 * blackbox_module_init- top-level initialization for the Blackbox driver
 *
 * This is normally invoked when other drivers are being initialized, but
 * it can be manually invoked in cases where very early crashes are to be
 * logged, i.e. when CONFIG_BLACKBOX_EARLY_INIT is enabled.
 */
int blackbox_module_init(void)
{
	pr_info("Say hi to Blackbox\n");
	/*
	 * If CONFIG_BLACKBOX_EARLY_INIT is set, we could be invoked from
	 * each class of Blackbox device, as well as during normal driver
	 * initialization. This is unavoidable, so make sure we only do the
	 * work of initialization once.
	 */
	if (initialized)
		return 0;

	initialized = true;

#if 0
	create_cmdline();
#endif
	return 0;
}
EXPORT_SYMBOL(blackbox_module_init);

void blackbox_module_exit(void)
{
	pr_info("So long to blackbox\n");
	initialized = false;
}
EXPORT_SYMBOL(blackbox_module_exit);

module_init(blackbox_module_init);
module_exit(blackbox_module_exit);
MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("David VomLehn");
