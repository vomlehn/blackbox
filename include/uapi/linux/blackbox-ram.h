/*
 * Definitions for RAM-based black box devices
 */

#ifndef _LINUX_BLACKBOX_RAM_H_
#define _LINUX_BLACKBOX_RAM_H_

#include <uapi/linux/blackbox.h>

#ifdef CONFIG_BLACKBOX_EARLY_INIT_RAM
extern void blackbox_early_init_ram(void);
#else
static inline void blackbox_early_init_ram(void)
{
}
#endif
#endif /* _LINUX_BLACKBOX_RAM_H_ */
