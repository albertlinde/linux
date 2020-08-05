// SPDX-License-Identifier: GPL-2.0-only
#include <linux/fault-inject.h>
#include <linux/fail-usercopy.h>
#include <linux/random.h>

static struct {
	struct fault_attr attr;
	u32 failsize;
} partial_usercopy = {
	.attr = FAULT_ATTR_INITIALIZER,
	.failsize = 0,
};

static int __init setup_partial_usercopy(char *str)
{
	return setup_fault_attr(&partial_usercopy.attr, str);
}
__setup("partial_usercopy=", setup_partial_usercopy);

#ifdef CONFIG_FAULT_INJECTION_DEBUG_FS

static int __init partial_usercopy_debugfs(void)
{
	umode_t mode = S_IFREG | 0600;
	struct dentry *dir;

	dir = fault_create_debugfs_attr("partial_usercopy", NULL,
					&partial_usercopy.attr);
	if (IS_ERR(dir))
		return PTR_ERR(dir);

	debugfs_create_u32("failsize", mode, dir,
			   &partial_usercopy.failsize);
	return 0;
}

late_initcall(partial_usercopy_debugfs);

#endif /* CONFIG_FAULT_INJECTION_DEBUG_FS */

long should_partial_usercopy(unsigned long n)
{
	if (should_fail(&partial_usercopy.attr, n)) {
		if (partial_usercopy.failsize > 0) {
			return partial_usercopy.failsize % (n + 1);
		} else {
			return -EFAULT;
		}
	}
	return 0;
}
EXPORT_SYMBOL_GPL(should_partial_usercopy);
