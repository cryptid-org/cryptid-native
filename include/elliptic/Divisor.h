#ifndef __CRYPTID_DIVISOR_H
#define __CRYPTID_DIVISOR_H

#include "complex/Complex.h"
#include "elliptic/AffinePoint.h"
#include "elliptic/ComplexAffinePoint.h"
#include "elliptic/EllipticCurve.h"
#include "util/Status.h"


Complex divisor_evaluateVertical(EllipticCurve ec, AffinePoint a, ComplexAffinePoint b);

Status divisor_evaluateTangent(Complex* result, EllipticCurve ec, AffinePoint a, ComplexAffinePoint b);

Status divisor_evaluateLine(Complex* result, EllipticCurve ec, AffinePoint a, AffinePoint aprime, ComplexAffinePoint b);

#endif
