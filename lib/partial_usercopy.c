// SPDX-License-Identifier: GPL-2.0-only
#include <linux/partial_usercopy.h>

#ifdef CONFIG_PARTIAL_USERCOPY

#include <linux/random.h>
#include <linux/fault-inject.h>

static struct {
	struct fault_attr attr;
	u32 failsize;
} partial_copy_from_user = {
	.attr = FAULT_ATTR_INITIALIZER,
	.failsize = 0,
};

static int __init setup_partial_copy_from_user(char *str)
{
	return setup_fault_attr(&partial_copy_from_user.attr, str);
}
__setup("partial_usercopy=", setup_partial_copy_from_user);

#ifdef CONFIG_FAULT_INJECTION_DEBUG_FS

static int __init partial_copy_from_user_debugfs(void)
{
	umode_t mode = S_IFREG | 0600;
	struct dentry *dir;
	dir = fault_create_debugfs_attr("partial_usercopy", NULL,
					&partial_copy_from_user.attr);
	if (IS_ERR(dir))
		return PTR_ERR(dir);

	debugfs_create_u32("failsize", mode, dir, &partial_copy_from_user.failsize);
	return 0;
}

late_initcall(partial_copy_from_user_debugfs);

#endif /* CONFIG_FAULT_INJECTION_DEBUG_FS */

unsigned long __should_partial_copy_from_user(unsigned long n)
{
	if(partial_copy_from_user.failsize > 0){
		if(should_fail(&partial_copy_from_user.attr, n)){
			if (partial_copy_from_user.failsize > n) {
				return n;
			}
			return partial_copy_from_user.failsize;
		}
	} else { // allow for fail-nth
		if(should_fail(&partial_copy_from_user.attr, n)){
			return prandom_u32() % (n + 1) ;
	}
	return 0;
}

#else /* CONFIG_PARTIAL_USERCOPY */

unsigned long __should_partial_copy_from_user(unsigned long n)
{
	return 0;
}

#endif /* CONFIG_PARTIAL_USERCOPY */

EXPORT_SYMBOL_GPL(__should_partial_copy_from_user);
