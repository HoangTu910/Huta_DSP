#ifndef _Q15_H_
#define _Q15_H_

#include<cmath>
#include<climits>
#include<vector>
#include"../common/debug.h"

/*
    Currently using Q17.14 format with the range from [-65536, 65535]
*/

const int Q_BITS = 14;
const int Q_SCALE = 1 << Q_BITS;
const int TAN_TABLE_SIZE = 1024;
extern std::vector<int> tan_lookup_table;

void initialize_tan_table();
int q17_14_tan_from_table(double angle_rad);

namespace DSP_MATH{
    int q17_14_add(int a, int b);
    int q17_14_subtract(int a, int b);
    int q17_14_multiply(int a, int b);
    int q17_14_divide(int a, int b);
    int float_to_q17_14(double x);
    double q17_14_to_float(int q);
    int int_to_q17_14(int x);
    int q17_14_to_int(int q);
    int q17_14_tan(int q_input);

    template<typename T>
    std::vector<int> preprocessQNumber(const std::vector<T>& inputSignal) {
        std::vector<int> processedSignal;
        processedSignal.reserve(inputSignal.size());
        for (const T& sample : inputSignal) {
            processedSignal.push_back(float_to_q17_14(static_cast<double>(sample)));
        }
        return processedSignal;
    }
}

#endif