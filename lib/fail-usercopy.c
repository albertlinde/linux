// SPDX-License-Identifier: GPL-2.0-only
#include <linux/fail-usercopy.h>
#include <linux/fault-inject.h>
#include <linux/random.h>

static struct {
	struct fault_attr attr;
	u32 failsize;
} fail_usercopy = {
	.attr = FAULT_ATTR_INITIALIZER,
	.failsize = 0,
};

static int __init setup_fail_usercopy(char *str)
{
	return setup_fault_attr(&fail_usercopy.attr, str);
}
__setup("fail_usercopy=", setup_fail_usercopy);

#ifdef CONFIG_FAULT_INJECTION_DEBUG_FS

static int __init fail_usercopy_debugfs(void)
{
	umode_t mode = S_IFREG | 0600;
	struct dentry *dir;

	dir = fault_create_debugfs_attr("fail_usercopy", NULL,
					&fail_usercopy.attr);
	if (IS_ERR(dir))
		return PTR_ERR(dir);

	debugfs_create_u32("failsize", mode, dir,
			   &fail_usercopy.failsize);
	return 0;
}

late_initcall(fail_usercopy_debugfs);

#endif /* CONFIG_FAULT_INJECTION_DEBUG_FS */

long should_fail_usercopy(unsigned long n)
{
	if (should_fail(&fail_usercopy.attr, n)) {
		if (fail_usercopy.failsize > 0)
			return fail_usercopy.failsize % (n + 1);
		return -EFAULT;
	}
	return 0;
}
EXPORT_SYMBOL_GPL(should_fail_usercopy);
