#ifndef __CRYPTID_COMPLEX_AS_BINARY_H
#define __CRYPTID_COMPLEX_AS_BINARY_H

#include "complex/Complex.h"

/**
 * Represents a Complex numbe with binaries of real and imaginary.
 */
typedef struct ComplexAsBinary {
  /**
   * The binary representation of the real part.
   */
  void *real;

  /**
   * The length of the real part's binary representation.
   */
  size_t realLength;

  /**
   * The binary representation of the imaginary part.
   */
  void *imaginary;

  /**
   * The length of the imaginary part's binary representation.
   */
  size_t imaginaryLength;
} ComplexAsBinary;

/**
 * Initializes a new ComplexAsBinary with the binary representation of specified
 * real and imaginary parts.
 * @param complexAsBinaryOutput the ComplexAsBinary to be initialized
 * @param real the binary representation of the real part
 * @param realLength the lenght of the real part's binary representation
 * @param imaginary the binary representation of the imaginary part
 * @param imaginaryLength the length of the imaginary part's binary
 * representation
 */
void complexAsBinary_init(ComplexAsBinary *complexAsBinaryOutput,
                          const void *const real, const size_t realLength,
                          const void *const imaginary,
                          const size_t imaginaryLength);

/**
 * Frees a ComplexAsBinary. After calling this function on a ComplexAsBinary
 * instance, that instance should not be used anymore.
 * @param complexAsBinary the ComplexAsBinary to be destroyed
 */
void complexAsBinary_destroy(ComplexAsBinary complexAsBinary);

/**
 * Converts a ComplexAsBinary to Complex.
 * @param complexOutput the Complex to be initialized based on a ComplexAsBinary
 * @param complexAsBinary the ComplexAsBinary, which needs to be converted to
 * Complex
 */
void complexAsBinary_toComplex(Complex *complexOutput,
                               const ComplexAsBinary complexAsBinary);

/**
 * Converts a Complex to ComplexAsBinary.
 * @param complexAsBinaryOutput the ComplexAsBinary to be initialized based on a
 * Complex
 * @param complex the Complex, which needs to be converted to ComplexAsBinary
 */
void complexAsBinary_fromComplex(ComplexAsBinary *complexAsBinaryOutput,
                                 const Complex complex);

#endif