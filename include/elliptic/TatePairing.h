#ifndef __CRYPTID_TATEPAIRING_H
#define __CRYPTID_TATEPAIRING_H

#include "gmp.h"

#include "elliptic/AffinePoint.h"
#include "elliptic/ComplexAffinePoint.h"
#include "elliptic/EllipticCurve.h"
#include "complex/Complex.h"
#include "util/Status.h"


Status tate_performPairing(Complex *result, int embeddingDegree, EllipticCurve ellipticCurve, mpz_t subgroupOrder, AffinePoint p, AffinePoint b);

#endif
