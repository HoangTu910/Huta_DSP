#ifndef _QNUMBER_H_
#define _QNUMBER_H_

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#include<cmath>
#include<climits>
#include<vector>
#include"../common/debug.hpp"

/*
    Currently using Q1.15 format with the range from [-1, 0.999969482421875]
*/

const int Q_BITS = 15;
const int Q_SCALE = 1 << Q_BITS;
const int TAN_TABLE_SIZE = 1024;
const int SIN_TABLE_SIZE = 1024;
const int SATURATION_LIMIT_UPPER = 32767;
const int SATURATION_LIMIT_LOWER = -32768;

extern std::vector<int> tan_lookup_table;
extern std::vector<int> sin_lookup_table;

void initialize_tan_table();
void initialize_sin_table();
int q1_15_tan_from_table(double angle_rad);
int q1_15_sin_from_table(double angle_rad);

namespace DSP_MATH{
    int q1_15_add(int a, int b);
    int q1_15_subtract(int a, int b);
    int q1_15_multiply(int a, int b);
    int q1_15_divide(int a, int b);
    int float_to_q1_15(double x);
    double q1_15_to_float(int q);
    int int_to_q1_15(int x);
    int q1_15_to_int(int q);
    int q1_15_tan(int q_input);
    int q1_15_sin(int q_input);
    int q1_15_square(int q_input);
    int q1_15_sqrt(int q_input);
    int q1_15_pow(int q_base, int q_exp);

    template<typename T>
    std::vector<int> preprocessQNumber(const std::vector<T>& inputSignal) {
        std::vector<int> processedSignal;
        processedSignal.reserve(inputSignal.size());
        for (const T& sample : inputSignal) {
            processedSignal.push_back(float_to_q1_15(static_cast<double>(sample)));
        }
        return processedSignal;
    }
}

#endif