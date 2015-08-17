/*
 * Early initialize code for blackbox devices
 */

#include <linux/kernel.h>

#include <uapi/linux/blackbox.h>
#include <uapi/linux/blackbox-ram.h>

void blackbox_early_init(void)
{
	int rc;

	rc = blackbox_module_init();
	if (rc != 0)
		return rc;

	blackbox_early_init_ram();
	blackbox_early_init_nand();

	return 0;
}
