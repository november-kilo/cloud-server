#ifndef MATH_LIB_H

#define MATH_LIB_H

#include <float.h>
#include <kfun.h>
#include <type.h>
#include <Function.h>
#include <Iterator.h>

#define TOLERANCE 1e-7
#define MAX_ITERATIONS 250
#define MAX_INTEGRATION_ITERATIONS 20

#define Complex object "/lib/math/Complex"

#define Exponential object "/lib/math/functions/Exponential"
#define OneOverXSquared object "/lib/math/functions/OneOverXSquared"

#define Integrator object "/lib/math/integration/Integrator"
#define GaussLegendreIntegrator object "/lib/math/integration/GaussLegendreIntegrator"
#define RombergIntegrator object "/lib/math/integration/RombergIntegrator"
#define SimpsonIntegrator object "/lib/math/integration/SimpsonIntegrator"

#define Matrix object "/lib/math/matrix/Matrix"
#define EigenvalueDecomposition object "/lib/math/matrix/EigenvalueDecomposition"

#define Number object "/lib/math/Number"

#define Polynomial object "/lib/math/polynomial/Polynomial"
#define PolynomialDerivativeIterator object "/lib/math/polynomial/DerivativeIterator"
#define PolynomialInterpolator object "/lib/math/polynomial/Interpolator"
#define PolynomialProbabilityReducer object "/lib/math/polynomial/ProbabilityReducer"
#define PolynomialStringReducer object "/lib/math/polynomial/StringReducer"

#define Quadrature object "/lib/math/quadrature/Quadrature"
#define SimpsonQuad object "/lib/math/quadrature/Simpson"
#define TrapezoidQuadrature object "/lib/math/quadrature/Trapezoid"

#define Rational object "/lib/math/Rational"

#define Stats object "/lib/math/Stats"

#define Vector object "/lib/math/Vector"

#endif