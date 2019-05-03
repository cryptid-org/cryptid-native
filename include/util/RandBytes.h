#ifndef __CRYPTID_RAND_BYTES_H
#define __CRYPTID_RAND_BYTES_H

#include "util/Status.h"


/**
 * Fills the passed buffer from a cryptographically secure source.
 * 
 * If {@code (__CRYPTID_EXTERN_RANDOM} is defined, then this function will
 * call the {@code int __cryptid_cryptoRandom(void *buf, const int num)} function
 * internally.
 * @param buf the buffer to fill
 * @param num the size of the buffer
 * @return CRYPTID_SUCCESS if everything went right
 */
CryptidStatus randomBytes(unsigned char *buf, const int num);

#endif
