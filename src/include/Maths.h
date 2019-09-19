#ifndef MATHS_LIB_H

#define MATHS_LIB_H

#include <float.h>
#include <type.h>
#include <Function.h>
#include <Iterator.h>

#define TOLERANCE 1e-7
#define MAX_ITERATIONS 250
#define MAX_INTEGRATION_ITERATIONS 20

#define Complex object "/lib/maths/Complex"

#define Exponential object "/lib/maths/functions/Exponential"
#define OneOverXSquared object "/lib/maths/functions/OneOverXSquared"

#define Integrator object "/lib/maths/integration/Integrator"
#define GaussLegendreIntegrator object "/lib/maths/integration/GaussLegendreIntegrator"
#define RombergIntegrator object "/lib/maths/integration/RombergIntegrator"
#define SimpsonIntegrator object "/lib/maths/integration/SimpsonIntegrator"

#define Matrix object "/lib/maths/matrix/Matrix"
#define EigenvalueDecomposition object "/lib/maths/matrix/EigenvalueDecomposition"

#define Number object "/lib/maths/Number"

#define Polynomial object "/lib/maths/polynomial/Polynomial"
#define PolynomialDerivativeIterator object "/lib/maths/polynomial/DerivativeIterator"
#define PolynomialInterpolator object "/lib/maths/polynomial/Interpolator"
#define PolynomialProbabilityReducer object "/lib/maths/polynomial/ProbabilityReducer"
#define PolynomialStringReducer object "/lib/maths/polynomial/StringReducer"

#define Quadrature object "/lib/maths/quadrature/Quadrature"
#define SimpsonQuad object "/lib/maths/quadrature/Simpson"
#define TrapezoidQuadrature object "/lib/maths/quadrature/Trapezoid"

#define Rational object "/lib/maths/Rational"

#define Stats object "/lib/maths/Stats"

#define Vector object "/lib/maths/Vector"

#endif
