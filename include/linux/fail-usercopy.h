/* SPDX-License-Identifier: GPL-2.0 */

/*
 * This header provides a wrapper for the partial failures on
 * copy_{to,from}_user. The general idea is to have a method which
 * returns the amount of bytes copy_{to,from}_user should NOT copy
 * in this call, returning this value (in addition to any bytes
 * that could actually not be copied).
 */
#ifndef __LINUX_PARTIALCOPY_H__
#define __LINUX_PARTIALCOPY_H__

#ifdef CONFIG_FAULT_INJECTION_USERCOPY

/*
 * should_partial_usercopy returns a fault (negative) or a number
 * between 0 and n, inclusive.
 */
long should_partial_usercopy(unsigned long n);

#else

static inline long should_partial_usercopy(unsigned long n)
{
	return 0;
}

#endif /* CONFIG_FAULT_INJECTION_USERCOPY */

#endif /* __LINUX_PARTIALCOPY_H__ */
