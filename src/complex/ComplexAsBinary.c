#include <stdlib.h>
#include <string.h>

#include "gmp.h"

#include "complex/ComplexAsBinary.h"

void complexAsBinary_init(ComplexAsBinary *complexAsBinaryOutput,
                          const void *const real, const size_t realLength,
                          const void *const imaginary,
                          const size_t imaginaryLength) {
  complexAsBinaryOutput->real = malloc(realLength + 1);
  memcpy(complexAsBinaryOutput->real, real, realLength + 1);

  complexAsBinaryOutput->realLength = realLength;

  complexAsBinaryOutput->imaginary = malloc(imaginaryLength + 1);
  memcpy(complexAsBinaryOutput->imaginary, imaginary, imaginaryLength + 1);

  complexAsBinaryOutput->imaginaryLength = imaginaryLength;
}

void complexAsBinary_destroy(ComplexAsBinary complexAsBinary) {
  free(complexAsBinary.real);
  free(complexAsBinary.imaginary);
}

void complexAsBinary_toComplex(Complex *complexOutput,
                               const ComplexAsBinary complexAsBinary) {
  mpz_inits(complexOutput->real, complexOutput->imaginary, NULL);
  mpz_import(complexOutput->real, complexAsBinary.realLength, 1, 1, 0, 0,
             complexAsBinary.real);
  mpz_import(complexOutput->imaginary, complexAsBinary.imaginaryLength, 1, 1, 0,
             0, complexAsBinary.imaginary);
}

void complexAsBinary_fromComplex(ComplexAsBinary *complexAsBinaryOutput,
                                 const Complex complex) {
  complexAsBinaryOutput->real = mpz_export(
      NULL, &complexAsBinaryOutput->realLength, 1, 1, 0, 0, complex.real);

  complexAsBinaryOutput->imaginary =
      mpz_export(NULL, &complexAsBinaryOutput->imaginaryLength, 1, 1, 0, 0,
                 complex.imaginary);
}