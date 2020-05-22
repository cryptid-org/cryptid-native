#ifndef __CRYPTID_AFFINEPOINT_AS_BINARY_H
#define __CRYPTID_AFFINEPOINT_AS_BINARY_H

#include "elliptic/AffinePoint.h"

/**
 * ## Description
 *
 * Represents an affine point with binaries of coordinates.
 */
typedef struct AffinePointAsBinary {
  /**
   * ## Description
   *
   * The binary representation of the \f$x\f$ coordinate.
   */
  void *x;

  /**
   * ## Description
   *
   * The length of the x coordinate's binary representation.
   */
  size_t xLength;

  /**
   * ## Description
   *
   * The binary representation of the \f$y\f$ coordinate.
   */
  void *y;

  /**
   * ## Description
   *
   * The length of the y coordinate's binary representation.
   */
  size_t yLength;
} AffinePointAsBinary;

/**
 * ## Description
 *
 * Initializes a new AffinePointAsBinary with the binary representation of
 * specified arbitrary precision coordinates.
 *
 * ## Parameters
 *
 *   * affinePointAsBinaryOutput
 *     * The AffinePointAsBinary to be initialized.
 *   * x
 *     * The binary representation of the \f$x\f$ coordinate.
 *   * xLength
 *     * The lenght of the x coordinate's binary representation.
 *   * y
 *     * The binary representation of the \f$y\f$ coordinate.
 *   * yLength
 *     * The length of the y coordinate's binary representation.
 */
void affineAsBinary_init(AffinePointAsBinary *affinePointAsBinaryOutput,
                         const void *const x, const size_t xLength,
                         const void *const y, const size_t yLength);

/**
 * ## Description
 *
 * Frees an AffinePointAsBinary. After calling this function on an
 * AffinePointAsBinary instance, that instance should not be used anymore.
 *
 * ## Parameters
 *
 *   * affinePointAsBinary
 *     * The AffinePointAsBinary to be destroyed.
 */
void affineAsBinary_destroy(AffinePointAsBinary affinePointAsBinary);

/**
 * ## Description
 *
 * Converts an AffinePointAsBinary to AffinePoint.
 *
 * ## Parameters
 *
 *   * affinePointOutput
 *     * The AffinePoint to be initialized based on an AffinePointAsBinary.
 *   * affinePointAsBinary
 *     * The AffinePointAsBinary, which needs to be converted to AffinePoint.
 */
void affineAsBinary_toAffine(AffinePoint *affinePointOutput,
                             const AffinePointAsBinary affinePointAsBinary);

/**
 * ## Description
 *
 * Converts an AffinePoint to AffinePointAsBinary.
 *
 * ## Parameters
 *
 *   * affinePointAsBinaryOutput
 *     * The AffinePointAsBinary to be initialized based on an AffinePoint.
 *   * affinePoint
 *     * The AffinePoint, which needs to be converted to AffinePointAsBinary.
 */
void affineAsBinary_fromAffine(AffinePointAsBinary *affinePointAsBinaryOutput,
                               const AffinePoint affinePoint);

#endif