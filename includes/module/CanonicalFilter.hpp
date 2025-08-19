#ifndef _CANONICALFILTER_H_
#define _CANONICALFILTER_H_

#include "Filter.hpp"
#include "../dsp_math/q15.h"
#include <iostream>
#include <vector>
#include <cmath>
#include <numeric>

#define LOW_PASS_1 1
#define HIGH_PASS_1 2
#define ALL_PASS_1 3
#define LOW_PASS_2 4
#define HIGH_PASS_2 5
#define ALL_PASS_2 6
#define BAND_PASS_2 7
#define BAND_REJECT_2 8


const double Q = 0.707;

class CanonicalFilter : public Filter {
public:
    CanonicalFilter(int fs) {
        m_fs = DSP_MATH::int_to_q15(fs);
    };
    void setType(int type, int fc); //fc can be f_center or f_cut based on L-HP or B-P
    int process(int input) override;
    void process(std::vector<double> &inputSignal, std::vector<int> &outputSignal) override;

private:
    int m_fs, m_fc;
    int m_K;
    int m_b0, m_b1, m_b2;
    int m_a1, m_a2;
    int m_xh1, m_xh2;
};

#endif
