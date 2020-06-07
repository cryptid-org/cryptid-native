#ifndef __CRYPTID_COMPLEX_AS_BINARY_H
#define __CRYPTID_COMPLEX_AS_BINARY_H

#include "complex/Complex.h"

/**
 * ## Description
 *
 * Represents a Complex numbe with binaries of real and imaginary.
 */
typedef struct ComplexAsBinary {
  /**
   * ## Description
   *
   * The binary representation of the real part.
   */
  void *real;

  /**
   * ## Description
   *
   * The length of the real part's binary representation.
   */
  size_t realLength;

  /**
   * ## Description
   *
   * The binary representation of the imaginary part.
   */
  void *imaginary;

  /**
   * ## Description
   *
   * The length of the imaginary part's binary representation.
   */
  size_t imaginaryLength;
} ComplexAsBinary;

/**
 * ## Description
 *
 * Initializes a new ComplexAsBinary with the binary representation of specified
 * real and imaginary parts.
 *
 * ## Parameters
 *
 *   * complexAsBinaryOutput
 *     * The ComplexAsBinary to be initialized.
 *   * real
 *     * The binary representation of the real part.
 *   * realLength
 *     * The lenght of the real part's binary representation.
 *   * imaginary
 *     * The binary representation of the imaginary part.
 *   * imaginaryLength
 *     * The length of the imaginary part's binary representation.
 */
void complexAsBinary_init(ComplexAsBinary *complexAsBinaryOutput,
                          const void *const real, const size_t realLength,
                          const void *const imaginary,
                          const size_t imaginaryLength);

/**
 * ## Description
 *
 * Frees a ComplexAsBinary. After calling this function on a ComplexAsBinary
 * instance, that instance should not be used anymore.
 *
 * ## Parameters
 *
 *   * complexAsBinary
 *     * The ComplexAsBinary to be destroyed.
 */
void complexAsBinary_destroy(const ComplexAsBinary complexAsBinary);

/**
 * ## Description
 *
 * Converts a ComplexAsBinary to Complex.
 *
 * ## Parameters
 *
 *   * complexOutput
 *     * The Complex to be initialized based on a ComplexAsBinary.
 *   * complexAsBinary
 *     * The ComplexAsBinary, which needs to be converted to Complex.
 */
void complexAsBinary_toComplex(Complex *complexOutput,
                               const ComplexAsBinary complexAsBinary);

/**
 * ## Description
 *
 * Converts a Complex to ComplexAsBinary.
 *
 * ## Parameters
 *
 *   * complexAsBinaryOutput
 *     * The ComplexAsBinary to be initialized based on a Complex.
 *   * complex
 *     * The Complex, which needs to be converted to ComplexAsBinary.
 */
void complexAsBinary_fromComplex(ComplexAsBinary *complexAsBinaryOutput,
                                 const Complex complex);

#endif