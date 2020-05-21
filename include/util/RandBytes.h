#ifndef __CRYPTID_RAND_BYTES_H
#define __CRYPTID_RAND_BYTES_H

#include "util/Status.h"

/**
 * ## Description
 *
 * Fills the passed buffer from a cryptographically secure source.
 * If {@code (__CRYPTID_EXTERN_RANDOM} is defined, then this function will
 * call the {@code int __cryptid_cryptoRandom(void *buf, const int num)}
 * function internally.
 *
 * ## Parameters
 *
 *   * buf
 *     * The buffer to fill.
 *   * num
 *     * The size of the buffer.
 *
 * ## Return Value
 *
 * CRYPTID_SUCCESS if everything went right.
 */
CryptidStatus cryptid_randomBytes(unsigned char *buf, const int num);

#endif
