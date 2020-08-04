/* SPDX-License-Identifier: GPL-2.0 */

/*
 * This header provides a wrapper for the partial failures on
 * copy_from_user. The general idea is to have a method which
 * returns the amount of bytes copy_from_user should NOT copy
 * in this call, returning this value (in addition to any bytes
 * that could actually not be copied).
 */
#ifndef __LINUX_PARTIALCOPY_H__
#define __LINUX_PARTIALCOPY_H__

/*
 * __should_partial_copy_from_user always returns a number
 * between 0 and n, inclusive.
 */
unsigned long __should_partial_copy_from_user(unsigned long n);
static inline unsigned long should_partial_copy_from_user(unsigned long n)
{
	return  __should_partial_copy_from_user(n);
}

#endif /* __LINUX_PARTIALCOPY_H__ */
