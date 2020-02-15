#ifndef __CRYPTID_VALIDATION_H
#define __CRYPTID_VALIDATION_H

/**
 * Enumeration of the possible results of a validation process.
 */
typedef enum
{
    /*
     * Flipped values match boolean semantics better.
     */ 
    CRYPTID_VALIDATION_SUCCESS = 1,
    CRYPTID_VALIDATION_FAILURE = 0
} CryptidValidationResult;

#endif
