#pragma once

#include <cstdlib>
#include <cmath>
#include <float.h>

constexpr auto PI = 3.14159265358979323846f;

#define SQR(x) ((x)*(x))

#define DEG2RAD(a) ((a)*(PI/180.0))
#define RAD2DEG(r) ((r)*(180.0/PI))

#define SIN(a) (sin(DEG2RAD(a)))
#define COS(a) (cos(DEG2RAD(a)))
#define TAN(a) (tan(DEG2RAD(a)))

#define ACOS(x) (RAD2DEG(acos((x))))
#define ASIN(x) (RAD2DEG(asin((x))))
#define ATAN(x) (RAD2DEG(atan((x))))

#define ROUND1DPP(x) (((int)(((x) + .05) * 10)) / 10.0)
#define ROUND1DPN(x) (((int)(((x) - .05) * 10)) / 10.0)
#define ROUND2DPP(x) (((int)(((x) + .005) * 100)) / 100.0)
#define ROUND2DPN(x) (((int)(((x) - .005) * 100)) / 100.0)
#define ROUND3DPP(x) (((int)(((x) + .0005) * 1000)) / 1000.0)
#define ROUND3DPN(x) (((int)(((x) - .0005) * 1000)) / 1000.0)

#define ROUND1DP(x) (((x)>=0)?ROUND1DPP((x)):ROUND1DPN((x)))
#define ROUND2DP(x) (((x)>=0)?ROUND2DPP((x)):ROUND2DPN((x)))
#define ROUND3DP(x) (((x)>=0)?ROUND3DPP((x)):ROUND3DPN((x)))

// This needs to be at this level for the matrices
#ifndef CUSTOM_EPSILON
#define CUSTOM_EPSILON .000001
#define CUSTOM_EPSILON5DP .00001
#endif

#define SIGN(x) (((x)>=0)?1:-1)
#define IS_ZERO(x) (abs(x) <= DBL_EPSILON)
#define IS_ALMOST_ZERO(x) (abs(x) <= CUSTOM_EPSILON)
#define IS_ALMOST_ZERO5P(x) (abs(x) <= CUSTOM_EPSILON5DP)
#define ARE_EQUAL(x, y) (IS_ZERO((x)-(y)))
#define ARE_ALMOST_EQUAL(x, y) (IS_ALMOST_ZERO((x)-(y)))
#define ARE_ALMOST_EQUAL5DP(x, y) (IS_ALMOST_ZERO5P((x)-(y)))
#define NOT_EQUAL(x, y) (!ARE_ALMOST_EQUAL(x, y))
#define CHECK_ZERO(x) ((IS_ALMOST_ZERO((x)))?0:(x))

// Generates a random value from 0 to 1
#define RANDOM ((rand() % 1001) / 1000.0)
// Generates a random real values from -1 to 1
#define REAL_RANDOM_NEG (((rand() % 2001) - 1000) / 1000.0)
// Generates a random integer from low to high inclusive
#define RANGED_RANDOM_INT(low, high) ((rand() % ((high) - (low) + 1)) + (low))
// Generates a random double from low to high inclusive
#define RANGED_RANDOM(low, high) ((RANDOM * ((high)-(low))) + (low))
// Simulates a coin flip based on a given probability
#define FLIP(probability) (((probability) >= 1.0)?true:(RANDOM<=(probability)))