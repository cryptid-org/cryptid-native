#ifndef DIVISOR_H
#define DIVISOR_H

#include "AffinePoint.h"
#include "ComplexAffinePoint.h"
#include "Complex.h"
#include "EllipticCurve.h"
#include "Status.h"

Complex divisor_evaluateVertical(EllipticCurve ec, AffinePoint a, ComplexAffinePoint b);

Status divisor_evaluateTangent(Complex* result, EllipticCurve ec, AffinePoint a, ComplexAffinePoint b);

Status divisor_evaluateLine(Complex* result, EllipticCurve ec, AffinePoint a, AffinePoint aprime, ComplexAffinePoint b);

#endif
