#ifndef MATHS_LIB_H

#define MATHS_LIB_H

#include <float.h>
#include <type.h>
#include <Function.h>
#include <Iterator.h>

#define TOLERANCE 1e-7
#define MAX_ITERATIONS 250

#define Complex object "/lib/maths/Complex"

#define Integrator object "/lib/maths/integration/Integrator"
#define SimpsonIntegrator object "/lib/maths/integration/SimpsonIntegrator"

#define Matrix object "/lib/maths/matrix/Matrix"
#define EigenvalueDecomposition object "/lib/maths/matrix/EigenvalueDecomposition"

#define Number object "/lib/maths/Number"

#define Polynomial object "/lib/maths/polynomial/Polynomial"
#define PolynomialDerivativeIterator object "/lib/maths/polynomial/DerivativeIterator"
#define PolynomialProbabilityReducer object "/lib/maths/polynomial/ProbabilityReducer"
#define PolynomialStringReducer object "/lib/maths/polynomial/StringReducer"

#define Rational object "/lib/maths/Rational"

#define Stats object "/lib/maths/Stats"

#define Vector object "/lib/maths/Vector"

#endif