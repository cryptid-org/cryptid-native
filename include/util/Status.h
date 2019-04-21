#ifndef __CRYPTID_STATUS_H
#define __CRYPTID_STATUS_H

typedef enum Status
{
    SUCCESS,

    /*
     * 1 Could not calculate the multiplicative inverse.
     */
    HAS_NO_MUL_INV_ERROR,

    /*
     * 2 Can not calculate the divisor of a line if a given point is infinity.
     */
    DIVISOR_OF_LINE_INFINITY_ERROR,

    /*
     * 3 Can not calculate the divisor of a tangent if a given point is infinity.
     */
    DIVISOR_OF_TANGENT_INFINITY_ERROR,

    /*
     * 4 The message must not be null.
     */
    MESSAGE_NULL_ERROR,

    /*
     * 5 The message's length must not be 0.
     */
    MESSAGE_LENGTH_ERROR,

    /*
     * 6 The identity must not be null.
     */
    IDENTITY_NULL_ERROR,

    /*
     * 7 The identity's length must not be 0.
     */
    IDENTITY_LENGTH_ERROR,

    /*
     * 8 The given public parameters are incorrect.
     */
    ILLEGAL_PUBLIC_PARAMETERS_ERROR,

    /*
     * 9 The given private key is incorrect.
     */
    ILLEGAL_PRIVATE_KEY_ERROR,

    /*
     * 10 The given ciphertext tuple is incorrect.
     */
    ILLEGAL_CIPHERTEXT_TUPLE_ERROR,
    
    /*
     * 11 Failed to decrypt the ciphertext, the input arguments might be incorrect.
     */
    DECRYPTION_FAILED_ERROR,

    /*
     * 12 An algorithm has reached the maximum number of attempts while performing some calculation.
     */
    ATTEMPT_LIMIT_REACHED_ERROR,

    /*
     * 13 The pointer to the public parameters was null.
     */
    PUBLIC_PARAMETERS_NULL_ERROR,

    /*
     * 14 The pointer to the result was null.
     */
    RESULT_POINTER_NULL_ERROR,

    /*
     * 15 Could not generate random stuff.
     */
    RANDOM_GENERATION_ERROR
} Status;

#endif
