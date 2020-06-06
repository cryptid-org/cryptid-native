/**
 * # Binary Representation of Arbitrary Precision Complex Number
 *
 * ## Description
 *
 * Binary representation of [Complex](codebase://complex/Complex.h#Complex) for easier serialization and deserialization. Also includes the necessary conversion functions.
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
 *
 * ### Conversion
 *
 * Functions supporting the conversion of
 * [ComplexAsBinary](codebase://complex/ComplexAsBinary.h#ComplexAsBinary) and [Complex](codebase://complex/Complex.h#Complex) back and forth.
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
   * The binary representation of [Complex.real](codebase://complex/Complex.h#Complex.real).
   */
  void *real;

  /**
   * ## Description
   *
   * The length of [real](codebase://complex/ComplexAsBinary.h#ComplexAsBinary.real).
   */
  size_t realLength;

  /**
   * ## Description
   *
   * The binary representation of [Complex.imaginary](codebase://complex/Complex.h#Complex.imaginary).
   */
  void *imaginary;

  /**
   * ## Description
   *
   * The length of [imaginary](codebase://complex/ComplexAsBinary.h#ComplexAsBinary.imaginary).
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
void complexAsBinary_destroy(ComplexAsBinary complexAsBinary);

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