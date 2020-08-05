// SPDX-License-Identifier: GPL-2.0-only
#include <linux/fault-inject.h>
#include <linux/fault-inject-usercopy.h>
#include <linux/random.h>

static struct {
	struct fault_attr attr;
} fail_usercopy = {
	.attr = FAULT_ATTR_INITIALIZER,
};

static int __init setup_fail_usercopy(char *str)
{
	return setup_fault_attr(&fail_usercopy.attr, str);
}
__setup("fail_usercopy=", setup_fail_usercopy);

#ifdef CONFIG_FAULT_INJECTION_DEBUG_FS

static int __init fail_usercopy_debugfs(void)
{
	struct dentry *dir;

	dir = fault_create_debugfs_attr("fail_usercopy", NULL,
					&fail_usercopy.attr);
	if (IS_ERR(dir))
		return PTR_ERR(dir);

	return 0;
}

late_initcall(fail_usercopy_debugfs);

#endif /* CONFIG_FAULT_INJECTION_DEBUG_FS */

/**
 * should_fail_usercopy() - Failure code or amount of bytes not to copy.
 * @n: Size of the original copy call.
 *
 * The general idea is to have a method which returns the amount of bytes not
 * to copy, a failure to return, or 0 if the calling function should progress
 * without a failure. E.g., copy_{to,from}_user should NOT copy the amount of
 * bytes returned by should_fail_usercopy, returning this value (in addition
 * to any bytes that could actually not be copied) or a failure.
 *
 * Return: one of:
 * negative, failure to return;
 * 0, progress normally;
 * a number in ]0, n], the number of bytes not to copy.
 *
 */
long should_fail_usercopy(unsigned long n)
{
	unsigned int fail_size;

	if (in_task()) {
		if (should_fail(&fail_usercopy.attr, n)) {
			fail_size = READ_ONCE(current->fail_usercopy_size);
			if (fail_size != 0)
				return fail_size < n ? fail_size : n;
			return -EFAULT;
		}
	}
	return 0;
}
EXPORT_SYMBOL_GPL(should_fail_usercopy);