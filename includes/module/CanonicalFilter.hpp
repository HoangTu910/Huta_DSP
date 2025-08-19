#ifndef _CANONICALFILTER_H_
#define _CANONICALFILTER_H_

#include "../interface/IFilter.hpp"
#include "../common/debug.h"
#include "../common/filterType.h"
#include "../dsp_math/QNumber.h"
#include <iostream>
#include <vector>
#include <cmath>
#include <numeric>

namespace Filter {
class CanonicalFilter : public IFilter {
public:
    CanonicalFilter(int fs) {
        m_fs = DSP_MATH::int_to_q17_14(fs);
        debug(m_fs);
        m_xh1 = DSP_MATH::q17_14_to_int(0);
        m_xh2 = DSP_MATH::q17_14_to_int(0);
    };
    void setType(int type, int fc, double Q); //fc can be f_center or f_cut based on L-HP or B-P
    int process(int input) override;
    void process(std::vector<double> &inputSignal, std::vector<int> &outputSignal) override;

private:
    int m_fs, m_fc;
    int m_K;
    int m_b0, m_b1, m_b2;
    int m_a1, m_a2;
    int m_xh1, m_xh2;
};
};

#endif
