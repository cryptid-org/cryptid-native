#include "elliptic/TatePairing.h"
#include "elliptic/Divisor.h"

// References:
//   * [Intro-to-IBE] Luther Martin. 2008. Introduction to Identity-Based
//   Encryption (Information Security and Privacy Series) (1 ed.). Artech House,
//   Inc., Norwood, MA, USA.

CryptidStatus tate_performPairing(Complex *result, const AffinePoint p,
                                  const AffinePoint b,
                                  const int embeddingDegree,
                                  const mpz_t subgroupOrder,
                                  const EllipticCurve ellipticCurve) {
  // Implementation of Miller's algorithm as it's written on this page:
  // https://crypto.stanford.edu/pbc/notes/ep/miller.html
  ComplexAffinePoint q;

  // Distortion map - Creates linearly independent points
  // For examples on distortion maps, see [Intro-to-IBE p63.].
  //
  // Here we use a Xi distortion map, which involves calculating a \f$\xi\f$
  // value. For Type-1 elliptic curves, this is calculated as follows: \f$\xi =
  // \frac{p - 1}{2}(1 + 3^{\frac{p + 1}{4}}i)\f$ where \f$p\f$ is the field
  // order of the elliptic curve field.
  if (affine_isInfinity(b)) {
    q = complexAffine_infinity();
  } else {
    mpz_t axi, bxi, three, one, addition, quotient, difference;
    mpz_inits(axi, bxi, three, one, addition, quotient, difference, NULL);
    Complex xi, tmp, xprime;

    mpz_sub_ui(difference, ellipticCurve.fieldOrder, 1);
    mpz_cdiv_q_ui(axi, difference, 2);

    mpz_set_ui(three, 3);
    mpz_add_ui(addition, ellipticCurve.fieldOrder, 1);
    mpz_cdiv_q_ui(quotient, addition, 4);
    mpz_powm(bxi, three, quotient, ellipticCurve.fieldOrder);

    mpz_set_ui(one, 1);
    complex_initMpz(&tmp, one, bxi);
    complex_modMulInteger(&xi, axi, tmp, ellipticCurve.fieldOrder);

    // \f$x^{\prime} = x \cdot xi\f$
    // \f$x \in \f$F_p\f$ | \f$xi\f$ \in \f$F_p^2\f$
    //
    // Here we assume, that we have to convert \f$x\f$ to \f$F_p^2\f$ and then
    // perform the multiplication according to the complex multiplication rules.
    complex_modMulInteger(&xprime, b.x, xi, ellipticCurve.fieldOrder);

    mpz_t zero;
    mpz_init_set_ui(zero, 0);
    Complex bY;
    complex_initMpz(&bY, b.y, zero);

    complexAffine_init(&q, xprime, bY);

    complex_destroy(bY);
    mpz_clear(zero);

    mpz_clears(axi, bxi, three, one, addition, quotient, difference, NULL);
    complex_destroyMany(3, xi, tmp, xprime);
  }

  if (complexAffine_isInfinity(q)) {
    complexAffine_destroy(q);
    complex_initLong(result, 1, 0);
    return CRYPTID_SUCCESS;
  }

  // Now p and q are linearly indenependent.
  // Here we start the actual Miller's algorithm.
  Complex f, gVVQ, g2VMinus2VQ, g2VMinus2VQInv, frac, tmpF, gVPQ, gVPlusQ,
      gVPlusQInv;
  AffinePoint v, doubleV, vPlusP;

  // 1. Set \f$f\f$ = 1 and \f$v\f$ = \f$p\f$
  complex_initLong(&f, 1, 0);
  affine_init(&v, p.x, p.y);

  // 2. {@code for i = t - 1 to 0 do:}
  // where \f$t\f$ is the bitcount of the subgroup order.
  // Note, that we have to subtract 2 because of the allocation behavior
  // of GMP (please refer to
  // https://gmplib.org/manual/Miscellaneous-Integer-Functions.html).
  for (int i = mpz_sizeinbase(subgroupOrder, 2) - 2; i >= 0; --i) {
    // Double step
    // \f$f = f^{2} \frac{g_{v, v}(q)}{g_{2v, -2v}(q)}\f$
    CryptidStatus status = affine_add(&doubleV, v, v, ellipticCurve);
    if (status) {
      complexAffine_destroy(q);
      complex_destroy(f);
      affine_destroy(v);
      return status;
    }
    status = divisor_evaluateTangent(&gVVQ, v, q, ellipticCurve);
    if (status) {
      complexAffine_destroy(q);
      complex_destroy(f);
      affine_destroy(v);
      affine_destroy(doubleV);
      return status;
    }
    divisor_evaluateVertical(&g2VMinus2VQ, doubleV, q, ellipticCurve);
    status = complex_multiplicativeInverse(&g2VMinus2VQInv, g2VMinus2VQ,
                                           ellipticCurve.fieldOrder);
    if (status) {
      complexAffine_destroy(q);
      affine_destroy(v);
      affine_destroy(doubleV);
      complex_destroyMany(3, f, gVVQ, g2VMinus2VQ);
      return status;
    }
    complex_modMul(&frac, gVVQ, g2VMinus2VQInv, ellipticCurve.fieldOrder);
    complex_modMul(&tmpF, f, f, ellipticCurve.fieldOrder);

    complex_destroy(f);
    complex_modMul(&f, tmpF, frac, ellipticCurve.fieldOrder);

    affine_destroy(v);
    // \f$v = 2v\f$
    affine_init(&v, doubleV.x, doubleV.y);

    complex_destroyMany(5, gVVQ, g2VMinus2VQ, g2VMinus2VQInv, frac, tmpF);
    affine_destroy(doubleV);

    if (mpz_tstbit(subgroupOrder, i)) {
      // Add step
      // \f$f = f \frac{g_{v, p}(q)}{g_{v + p, -(b + p)}(q)}\f$
      status = affine_add(&vPlusP, v, p, ellipticCurve);
      if (status) {
        complexAffine_destroy(q);
        complex_destroy(f);
        affine_destroy(v);
        return status;
      }

      status = divisor_evaluateLine(&gVPQ, v, p, q, ellipticCurve);
      if (status) {
        complexAffine_destroy(q);
        complex_destroy(f);
        affine_destroy(v);
        affine_destroy(vPlusP);
        return status;
      }

      divisor_evaluateVertical(&gVPlusQ, vPlusP, q, ellipticCurve);

      status = complex_multiplicativeInverse(&gVPlusQInv, gVPlusQ,
                                             ellipticCurve.fieldOrder);
      if (status) {
        complexAffine_destroy(q);
        affine_destroy(v);
        affine_destroy(vPlusP);
        complex_destroyMany(3, f, gVPQ, gVPlusQ);
        return status;
      }
      complex_modMul(&frac, gVPQ, gVPlusQInv, ellipticCurve.fieldOrder);
      complex_modMul(&tmpF, f, frac, ellipticCurve.fieldOrder);

      complex_destroy(f);
      complex_initMpz(&f, tmpF.real, tmpF.imaginary);

      affine_destroy(v);
      // \f$v = v + p\f$
      affine_init(&v, vPlusP.x, vPlusP.y);

      complex_destroyMany(5, gVPQ, gVPlusQ, gVPlusQInv, frac, tmpF);
      affine_destroy(vPlusP);
    }
  }
  affine_destroy(v);
  complexAffine_destroy(q);

  // Final Exponentiation
  mpz_t exponent, pPow, exponentPart;
  mpz_inits(exponent, pPow, exponentPart, NULL);

  mpz_pow_ui(pPow, ellipticCurve.fieldOrder, embeddingDegree);
  mpz_sub_ui(exponentPart, pPow, 1);
  mpz_cdiv_q(exponent, exponentPart, subgroupOrder);

  complex_modPow(result, f, exponent, ellipticCurve.fieldOrder);

  mpz_clears(exponent, pPow, exponentPart, NULL);
  complex_destroy(f);

  return CRYPTID_SUCCESS;
}
