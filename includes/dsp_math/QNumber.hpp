#ifndef _QNUMBER_H_
#define _QNUMBER_H_

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#include <cmath>
#include <climits>
#include <vector>
#include <cstdint>
#include "../common/debug.hpp"

/*
    Using Q16.15 format: signed 32-bit fixed point with 16 integer bits and
    15 fractional bits. Scale = 2^15. Representable range ~= [-32768, 32767.99997]
    but stored in 32-bit integers so saturation limits use 32-bit bounds.
*/

const int Q_BITS = 15;
const int Q_SCALE = 1 << Q_BITS;
const int TAN_TABLE_SIZE = 1024;
const int SIN_TABLE_SIZE = 1024;
const int SATURATION_LIMIT_UPPER = INT32_MAX;
const int SATURATION_LIMIT_LOWER = INT32_MIN;

extern std::vector<int> tan_lookup_table;
extern std::vector<int> sin_lookup_table;

void initialize_tan_table();
void initialize_sin_table();
int q16_15_tan_from_table(double angle_rad);
int q16_15_sin_from_table(double angle_rad);

namespace DSP_MATH{
    int q16_15_add(int a, int b);
    int q16_15_subtract(int a, int b);
    int q16_15_multiply(int a, int b);
    int q16_15_divide(int a, int b);
    int float_to_q16_15(double x);
    double q16_15_to_float(int q);
    int int_to_q16_15(int x);
    int q16_15_to_int(int q);
    int q16_15_tan(int q_input);
    int q16_15_sin(int q_input);
    int q16_15_square(int q_input);
    int q16_15_sqrt(int q_input);
    int q16_15_pow(int q_base, int q_exp);

    template<typename T>
    std::vector<int> preprocessQNumber(const std::vector<T>& inputSignal) {
        std::vector<int> processedSignal;
        processedSignal.reserve(inputSignal.size());
        for (const T& sample : inputSignal) {
            processedSignal.push_back(float_to_q16_15(static_cast<double>(sample)));
        }
        return processedSignal;
    }
}

#endif