#ifndef TATEPAIRING_H
#define TATEPAIRING_H

#include "gmp.h"

#include "AffinePoint.h"
#include "ComplexAffinePoint.h"
#include "Complex.h"
#include "EllipticCurve.h"
#include "Status.h"

Status tate_performPairing(Complex *result, int embeddingDegree, EllipticCurve ellipticCurve, mpz_t subgroupOrder, AffinePoint p, AffinePoint b);

#endif
