/* SPDX-License-Identifier: GPL-2.0 */

/*
 * This header provides a wrapper for partial failures on usercopy functions.
 * For usage see Documentation/fault-injection/fault-injection.rst
 */
#ifndef __LINUX_FAULT_INJECT_USERCOPY_H__
#define __LINUX_FAULT_INJECT_USERCOPY_H__

#ifdef CONFIG_FAULT_INJECTION_USERCOPY

long should_fail_usercopy(unsigned long n);

#else

static inline long should_fail_usercopy(unsigned long n) { return 0; }

#endif /* CONFIG_FAULT_INJECTION_USERCOPY */

#endif /* __LINUX_FAULT_INJECT_USERCOPY_H__ */