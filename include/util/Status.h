#ifndef __CRYPTID_STATUS_H
#define __CRYPTID_STATUS_H

/**
 * ## Description
 *
 * The result statuses of the library for success and errors.
 */
typedef enum {
  CRYPTID_SUCCESS,

  /*
   * ## Description
   *
   * Could not calculate the multiplicative inverse.
   */
  CRYPTID_HAS_NO_MUL_INV_ERROR,

  /*
   * ## Description
   *
   * Can not calculate the divisor of a line if a given point is infinity.
   */
  CRYPTID_DIVISOR_OF_LINE_INFINITY_ERROR,

  /*
   * ## Description
   *
   * Can not calculate the divisor of a tangent if a given point is infinity.
   */
  CRYPTID_DIVISOR_OF_TANGENT_INFINITY_ERROR,

  /*
   * ## Description
   *
   * The message must not be null.
   */
  CRYPTID_MESSAGE_NULL_ERROR,

  /*
   * ## Description
   *
   * The message's length must not be 0.
   */
  CRYPTID_MESSAGE_LENGTH_ERROR,

  /*
   * ## Description
   *
   * The identity must not be null.
   */
  CRYPTID_IDENTITY_NULL_ERROR,

  /*
   * ## Description
   *
   * The identity's length must not be 0.
   */
  CRYPTID_IDENTITY_LENGTH_ERROR,

  /*
   * ## Description
   *
   * The given public parameters are incorrect.
   */
  CRYPTID_ILLEGAL_PUBLIC_PARAMETERS_ERROR,

  /*
   * ## Description
   *
   * The given private key is incorrect.
   */
  CRYPTID_ILLEGAL_PRIVATE_KEY_ERROR,

  /*
   * ## Description
   *
   * The given ciphertext is incorrect.
   */
  CRYPTID_ILLEGAL_CIPHERTEXT_ERROR,

  /*
   * ## Description
   *
   * Failed to decrypt the ciphertext, the input arguments might be incorrect.
   */
  CRYPTID_DECRYPTION_FAILED_ERROR,

  /*
   * ## Description
   *
   * An algorithm has reached the maximum number of attempts while performing
   * some calculation.
   */
  CRYPTID_ATTEMPT_LIMIT_REACHED_ERROR,

  /*
   * The pointer to the public parameters was null.
   */
  CRYPTID_PUBLIC_PARAMETERS_NULL_ERROR,

  /*
   * ## Description
   *
   * The pointer to the result was null.
   */
  CRYPTID_RESULT_POINTER_NULL_ERROR,

  /*
   * ## Description
   *
   * Could not generate random stuff.
   */
  CRYPTID_RANDOM_GENERATION_ERROR,

  /*
   * ## Description
   *
   * The digital signature might be incorrect.
   */
  CRYPTID_VERIFICATION_FAILED_ERROR,

  /*
   * ## Description
   *
   * The given signature is incorrect.
   */
  CRYPTID_ILLEGAL_SIGNATURE_ERROR,

  /*
   * ## Description
   *
   * The given output parameter is NULL.
   */
  CRYPTID_HASH_NULLPOINTER_OUTPUT_PARAM_ERROR,

  /*
   * ## Description
   *
   * The given hash type is invalid.
   */
  CRPYTID_UNKNOWN_HASH_TYPE_ERROR
} CryptidStatus;

#endif
