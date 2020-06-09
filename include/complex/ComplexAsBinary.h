/**
 * # Binary Representation of Arbitrary Precision Complex Number
 *
 * ## Description
 *
 * Binary representation of [Complex](codebase://complex/Complex.h#Complex) for
 * easier serialization and deserialization. Also includes the necessary
 * conversion functions.
 *
 * ## Groups
 *
 * ### Initialization
 *
 * Functions supporting the initialization of a
 * [ComplexAsBinary](codebase://complex/ComplexAsBinary.h#ComplexAsBinary).
 *
 * ### Deinitialization
 *
 * Functions supporting the memory release of a
 * [ComplexAsBinary](codebase://complex/ComplexAsBinary.h#ComplexAsBinary).
 */
#ifndef __CRYPTID_COMPLEX_AS_BINARY_H
#define __CRYPTID_COMPLEX_AS_BINARY_H

#include "complex/Complex.h"

/**
 * ## Description
 *
 * Binary representation of a [Complex](codebase://complex/Complex.h#Complex).
 *
 * ## See Also
 *
 *   * codebase://complex/Complex.h#Complex
 */
typedef struct ComplexAsBinary {
  /**
   * ## Description
   *
   * The binary representation of
   * [Complex.real](codebase://complex/Complex.h#Complex.real).
   */
  void *real;

  /**
   * ## Description
   *
   * The length of
   * [real](codebase://complex/ComplexAsBinary.h#ComplexAsBinary.real).
   */
  size_t realLength;

  /**
   * ## Description
   *
   * The binary representation of
   * [Complex.imaginary](codebase://complex/Complex.h#Complex.imaginary).
   */
  void *imaginary;

  /**
   * ## Description
   *
   * The length of
   * [imaginary](codebase://complex/ComplexAsBinary.h#ComplexAsBinary.imaginary).
   */
  size_t imaginaryLength;
} ComplexAsBinary;

/**
 * ---
 * group: Initialization
 * ---
 *
 * ## Description
 *
 * Initializes a new complex number represented with binary values ($`real +
 * imaginary \cdot i`$).
 *
 * ## Parameters
 *
 *   * complexAsBinaryOutput
 *     * The pointer to the binary represented complex number to be initialized.
 * No memory allocation required before the function call. Should be destroyed
 * by the called if not needed anymore.
 *   * real
 *     * The binary representation of the real part $`a`$ of the complex number
 * $`a + bi`$.
 *   * realLength
 *     * The lenght of the real part's binary representation.
 *   * imaginary
 *     * The binary representation of the imaginary part $`b`$ of the complex
 * number $`a + bi`$.
 *   * imaginaryLength
 *     * The length of the imaginary part's binary representation.
 *
 * ## See Also
 *
 *   * codebase://complex/ComplexAsBinary.h#complexAsBinary_toComplex
 *   * codebase://complex/ComplexAsBinary.h#complexAsBinary_fromComplex
 *   * codebase://complex/ComplexAsBinary.h#complexAsBinary_destroy
 */
void complexAsBinary_init(ComplexAsBinary *complexAsBinaryOutput,
                          const void *const real, const size_t realLength,
                          const void *const imaginary,
                          const size_t imaginaryLength);

/**
 * ---
 * group: Deinitialization
 * ---
 *
 * ## Description
 *
 * Frees a
 * [ComplexAsBinary](codebase://complex/ComplexAsBinary.h#ComplexAsBinary)
 * instance. After calling this function on the instance, that instance should
 * not be used before reinitialization.
 *
 * ## Parameters
 *
 *   * complexAsBinary
 *     * The instance to be destroyed.
 *
 * ## See Also
 *
 *   * codebase://complex/ComplexAsBinary.h#complexAsBinary_init
 *   * codebase://complex/ComplexAsBinary.h#complexAsBinary_toComplex
 *   * codebase://complex/ComplexAsBinary.h#complexAsBinary_fromComplex
 */
void complexAsBinary_destroy(const ComplexAsBinary complexAsBinary);

/**
 * ---
 * group: Initialization
 * ---
 *
 * ## Description
 *
 * Initializes a [Complex](codebase://complex/ComplexAsBinary.h#ComplexAsBinary)
 * instance based on the given
 * [ComplexAsBinary](codebase://complex/ComplexAsBinary.h#ComplexAsBinary).
 *
 * ## Parameters
 *
 *   * complexOutput
 *     * The complex number to be initialized based on the given binary
 * representation. No memory allocation required before the function call.
 * Should be destroyed by the caller if not needed anymore.
 *   * complexAsBinary
 *     * The binary representation, which the initialization based on.
 *
 * ## See Also
 *
 *   * codebase://complex/ComplexAsBinary.h#complexAsBinary_init
 *   * codebase://complex/ComplexAsBinary.h#complexAsBinary_fromComplex
 *   * codebase://complex/ComplexAsBinary.h#complexAsBinary_destroy
 */
void complexAsBinary_toComplex(Complex *complexOutput,
                               const ComplexAsBinary complexAsBinary);

/**
 * ---
 * group: Initialization
 * ---
 *
 * ## Description
 *
 * Initializes a
 * [ComplexAsBinary](codebase://complex/ComplexAsBinary.h#ComplexAsBinary)
 * instance based on the given
 * [Complex](codebase://complex/ComplexAsBinary.h#ComplexAsBinary).
 *
 * ## Parameters
 *
 *   * complexAsBinaryOutput
 *     * The binary representation to be initialized based on a given complex
 * number.
 *   * complex
 *     * The complex number, which the initialization based on.
 *
 * ## See Also
 *
 *   * codebase://complex/ComplexAsBinary.h#complexAsBinary_init
 *   * codebase://complex/ComplexAsBinary.h#complexAsBinary_toComplex
 *   * codebase://complex/ComplexAsBinary.h#complexAsBinary_destroy
 */
void complexAsBinary_fromComplex(ComplexAsBinary *complexAsBinaryOutput,
                                 const Complex complex);

#endif