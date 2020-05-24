#ifndef __CRYPTID_EQUALITY_H
#define __CRYPTID_EQUALITY_H

/**
 * ## Description
 *
 * Enumeration of the possible results of an equality check. Flipped values to match boolean semantics better.
 */
typedef enum {
  CRYPTID_EQUAL = 1,
  CRYPTID_NOT_EQUAL = 0
} CryptidEqualityResult;

#endif
