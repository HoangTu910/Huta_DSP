#ifndef _Q15_H_
#define _Q15_H_

#include<cmath>
#include<climits>
#include<vector>

const int Q_BITS = 15;
const int Q_SCALE = 1 << Q_BITS;
const int TAN_TABLE_SIZE = 1024;
extern std::vector<int> tan_lookup_table;

void initialize_tan_table();
int q15_tan_from_table(double angle_rad);

namespace DSP_MATH{
    int q15_add(int a, int b);
    int q15_subtract(int a, int b);
    int q15_multiply(int a, int b);
    int q15_divide(int a, int b);
    int float_to_q15(double x);
    double q15_to_float(int q);
    int int_to_q15(int x);
    int q15_to_int(int q);
    int q15_tan(int q_input);

    template<typename T>
    std::vector<int> preprocessQ15(const std::vector<T>& inputSignal) {
        std::vector<int> processedSignal;
        processedSignal.reserve(inputSignal.size());
        for (const T& sample : inputSignal) {
            processedSignal.push_back(float_to_q15(static_cast<double>(sample)));
        }
        return processedSignal;
    }
}

#endif