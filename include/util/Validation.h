#ifndef __CRYPTID_VALIDATION_H
#define __CRYPTID_VALIDATION_H

/**
 * ## Description
 *
 * Enumeration of the possible results of a validation process. Flipped values
 * to match boolean semantics better.
 */
typedef enum {
  CRYPTID_VALIDATION_SUCCESS = 1,
  CRYPTID_VALIDATION_FAILURE = 0
} CryptidValidationResult;

#endif
