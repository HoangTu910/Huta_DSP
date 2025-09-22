#ifndef _COMMON_H_
#define _COMMON_H_

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#include <cmath>
#include <climits>
#include <vector>
#include <cstdint>
#include "../common/debug.hpp"

/*
    Some common mathematic number 
*/

#define N0_F 0.0f
#define N1_F 1.0f
#define N2_F 2.0f
#define N3_F 3.0f
#define N7_F 7.0f
#define N10_F 10.0f
#define N20_F 20.0f
#define N40_F 40.0f

#define COMMON_Q_FACTOR 0.7071067811865476f

enum FaderType {
    Linear,
    Exponential
};

#endif