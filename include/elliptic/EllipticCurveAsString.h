#ifndef __CRYPTID_ELLIPTICCURVE_AS_STRING_H
#define __CRYPTID_ELLIPTICCURVE_AS_STRING_H

#include"elliptic/EllipticCurve.h"

/**
 * A finite field on an elliptic curve of form \f$y^2 = x^3 + ax + b\f$ represented with string values.
 */
typedef struct EllipticCurveAsString
{
    char *a;

    int aBase;

    char *b;

    int bBase;

    char *fieldOrder;

    int fieldOrderBase;
} EllipticCurveAsString;

/**
 * Initializes a new EllipticCurve instance with the string representation of arbitrary precision integers in the specified bases.
 * @param ellipticCurveAsStringOutput the EllipticCurveAsString to be initialized
 * @param a the string representation of the coefficient \f$a\f$
 * @param aBase the base of the a coefficient's string representation
 * @param b the string representation of the coefficient \f$b\f$
 * @param bBase the base of the b coefficient's string representation
 * @param fieldOrder the string representation of the order of the finite field
 * @param fieldOrderBase the base of the finite field order's string representation
 */
void ellipticCurveAsString_init(EllipticCurveAsString *ellipticCurveAsStringOutput, const char *const a, const int aBase, const char *const b, const int bBase, const char *const fieldOrder, const int fieldOrderBase);

/**
 * Frees an EllipticCurveAsString.
 * @param ellipticCurveAsString the EllipticCurveAsString to be destroyed
 */
void ellipticCurveAsString_destroy(EllipticCurveAsString ellipticCurveAsString);

/**
 * Converts an EllipticCurveAsString to EllipticCurve.
 * @param ellipticCurveOutput the EllipticCurve to be initialized based on an EllipticCurveAsString
 * @param ellipticCurveAsString the EllipticCurveAsString, which needs to be converted to EllipticCurve
 */
void ellipticCurveAsString_toEllipticCurve(EllipticCurve *ellipticCurveOutput, const EllipticCurveAsString ellipticCurveAsString);

/**
 * Converts an EllipticCurve to EllipticCurveAsString.
 * @param ellipticCurveAsStringOutput the EllipticCurveAsString to be initialized based on an EllipticCurve
 * @param ellipticCurve the EllipticCurve, which needs to be converted to EllipticCurveAsString
 */
void ellipticCurve_toEllipticCurveAsString(EllipticCurveAsString *ellipticCurveAsStringOutput, const EllipticCurve ellipticCurve, const int aBase, const int bBase, const int fieldOrderBase);

#endif