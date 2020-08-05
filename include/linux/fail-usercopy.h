/* SPDX-License-Identifier: GPL-2.0 */

/*
 * This header provides a wrapper for partial failures on usercopy functions.
 * The general idea is to have a method which returns the amount of bytes not
 * to copy, a failure to return, or 0 if the calling function should progress
 * without a failure. E.g., copy_{to,from}_user should NOT copy the amount of
 * bytes returned by should_fail_usercopy, returning this value (in addition
 * to any bytes that could actually not be copied) or a failure.
 */
#ifndef __LINUX_PARTIALCOPY_H__
#define __LINUX_PARTIALCOPY_H__

#ifdef CONFIG_FAULT_INJECTION_USERCOPY

/*
 * should_fail_usercopy returns one of:
 * negative, failure to return;
 * 0, progress normally;
 * a number in ]0, n], the number of bytes not to copy.
 */
long should_fail_usercopy(unsigned long n);

#else

static inline long should_fail_usercopy(unsigned long n)
{
	return 0;
}

#endif /* CONFIG_FAULT_INJECTION_USERCOPY */

#endif /* __LINUX_PARTIALCOPY_H__ */
