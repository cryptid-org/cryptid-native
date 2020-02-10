#ifndef __CRYPTID_ELLIPTICCURVE_AS_BINARY_H
#define __CRYPTID_ELLIPTICCURVE_AS_BINARY_H

#include"elliptic/EllipticCurve.h"

/**
 * A finite field on an elliptic curve of form \f$y^2 = x^3 + ax + b\f$ represented with binary values.
 */
typedef struct EllipticCurveAsBinary
{
    char *a;

    size_t aLength;

    char *b;

    size_t bLength;

    char *fieldOrder;

    size_t fieldOrderLength;
} EllipticCurveAsBinary;

/**
 * Initializes a new EllipticCurve instance with the binary representation of arbitrary precision integers.
 * @param ellipticCurveAsBinaryOutput the EllipticCurveAsBinary to be initialized
 * @param a the binary representation of the coefficient \f$a\f$
 * @param aLength the length of the a coefficient's binary representation
 * @param b the binary representation of the coefficient \f$b\f$
 * @param bLength the length of the b coefficient's binary representation
 * @param fieldOrder the binary representation of the order of the finite field
 * @param fieldOrderLength the length of the finite field order's binary representation
 */
void ellipticCurveAsBinary_init(EllipticCurveAsBinary *ellipticCurveAsBinaryOutput, const char *const a, const size_t aLength, const char *const b, const size_t bLength, const char *const fieldOrder, const size_t fieldOrderLength);

/**
 * Frees an EllipticCurveAsBinary.
 * @param ellipticCurveAsBinary the EllipticCurveAsBinary to be destroyed
 */
void ellipticCurveAsBinary_destroy(EllipticCurveAsBinary ellipticCurveAsBinary);

/**
 * Converts an EllipticCurveAsBinary to EllipticCurve.
 * @param ellipticCurveOutput the EllipticCurve to be initialized based on an EllipticCurveAsBinary
 * @param ellipticCurveAsBinary the EllipticCurveAsBinary, which needs to be converted to EllipticCurve
 */
void ellipticCurveAsBinary_toEllipticCurve(EllipticCurve *ellipticCurveOutput, const EllipticCurveAsBinary ellipticCurveAsBinary);

/**
 * Converts an EllipticCurve to EllipticCurveAsBinary.
 * @param ellipticCurveAsBinaryOutput the EllipticCurveAsBinary to be initialized based on an EllipticCurve
 * @param ellipticCurve the EllipticCurve, which needs to be converted to EllipticCurveAsBinary
 */
void ellipticCurve_toEllipticCurveAsBinary(EllipticCurveAsBinary *ellipticCurveAsBinaryOutput, const EllipticCurve ellipticCurve);

#endif