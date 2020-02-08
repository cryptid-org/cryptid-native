#ifndef __CRYPTID_AFFINEPOINT_AS_STRING_H
#define __CRYPTID_AFFINEPOINT_AS_STRING_H

#include "elliptic/AffinePoint.h"

/**
 * Represents an affine point with strings of coordinates.
 */
typedef struct AffinePointAsString
{
    /**
     * The string representation of the \f$x\f$ coordinate.
     */
    char *x;

    /**
     * The base of the x coordinate's string representation.
     */
    int xBase;

    /**
     * The string representation of the \f$y\f$ coordinate.
     */
    char *y;

    /**
     * The base of the y coordinate's string representation.
     */
    int yBase;
} AffinePointAsString;

/**
 * Initializes a new AffinePointAsString with the string representation of specified arbitrary precision coordinates in the specified bases.
 * @param affinePointAsStringOutput the AffinePointAsString to be initialized
 * @param x the string representation of the \f$x\f$ coordinate
 * @param xBase the base of the x coordinate's string representation
 * @param y the string representation of the \f$y\f$ coordinate
 * @param yBase the base of the x coordinate's string representation
 */
void affineAsString_init(AffinePointAsString *affinePointAsStringOutput, const char *const x, const int xBase, const char *const y, const int yBase);

/**
 * Frees an AffinePointAsString. After calling this function on an AffinePointAsString instance,
 * that instance should not be used anymore.
 * @param affinePointAsString the AffinePointAsString to be destroyed
 */
void affineAsString_destroy(AffinePointAsString affinePointAsString);

/**
 * Converts an AffinePointAsString to AffinePoint.
 * @param affinePointOutput the AffinePoint to be initialized based on an AffinePointAsString
 * @param affinePointAsString the AffinePointAsString, which needs to be converted to AffinePoint
 */
void affineAsString_toAffine(AffinePoint *affinePointOutput, const AffinePointAsString affinePointAsString);

/**
 * Converts an AffinePoint to AffinePointAsString.
 * @param affinePointAsStringOutput the AffinePointAsString to be initialized based on an AffinePoint
 * @param affinePoint the AffinePoint, which needs to be converted to AffinePointAsString
 */
void affine_toAffineAsString(AffinePointAsString *affinePointAsStringOutput, const AffinePoint affinePoint);

#endif