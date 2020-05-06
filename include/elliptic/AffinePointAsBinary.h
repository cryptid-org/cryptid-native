#ifndef __CRYPTID_AFFINEPOINT_AS_BINARY_H
#define __CRYPTID_AFFINEPOINT_AS_BINARY_H

#include "elliptic/AffinePoint.h"

/**
 * Represents an affine point with binaries of coordinates.
 */
typedef struct AffinePointAsBinary {
  /**
   * The binary representation of the \f$x\f$ coordinate.
   */
  void *x;

  /**
   * The length of the x coordinate's binary representation.
   */
  size_t xLength;

  /**
   * The binary representation of the \f$y\f$ coordinate.
   */
  void *y;

  /**
   * The length of the y coordinate's binary representation.
   */
  size_t yLength;
} AffinePointAsBinary;

/**
 * Initializes a new AffinePointAsBinary with the binary representation of
 * specified arbitrary precision coordinates.
 * @param affinePointAsBinaryOutput the AffinePointAsBinary to be initialized
 * @param x the binary representation of the \f$x\f$ coordinate
 * @param xLength the lenght of the x coordinate's binary representation
 * @param y the binary representation of the \f$y\f$ coordinate
 * @param yLength the length of the y coordinate's binary representation
 */
void affineAsBinary_init(AffinePointAsBinary *affinePointAsBinaryOutput,
                         const void *const x, const size_t xLength,
                         const void *const y, const size_t yLength);

/**
 * Frees an AffinePointAsBinary. After calling this function on an
 * AffinePointAsBinary instance, that instance should not be used anymore.
 * @param affinePointAsBinary the AffinePointAsBinary to be destroyed
 */
void affineAsBinary_destroy(AffinePointAsBinary affinePointAsBinary);

/**
 * Converts an AffinePointAsBinary to AffinePoint.
 * @param affinePointOutput the AffinePoint to be initialized based on an
 * AffinePointAsBinary
 * @param affinePointAsBinary the AffinePointAsBinary, which needs to be
 * converted to AffinePoint
 */
void affineAsBinary_toAffine(AffinePoint *affinePointOutput,
                             const AffinePointAsBinary affinePointAsBinary);

/**
 * Converts an AffinePoint to AffinePointAsBinary.
 * @param affinePointAsBinaryOutput the AffinePointAsBinary to be initialized
 * based on an AffinePoint
 * @param affinePoint the AffinePoint, which needs to be converted to
 * AffinePointAsBinary
 */
void affineAsBinary_fromAffine(AffinePointAsBinary *affinePointAsBinaryOutput,
                               const AffinePoint affinePoint);

#endif