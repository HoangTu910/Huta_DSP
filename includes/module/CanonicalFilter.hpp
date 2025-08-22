#ifndef _CANONICALFILTER_H_
#define _CANONICALFILTER_H_

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#include "../interface/IFilter.hpp"
#include "../common/debug.hpp"
#include "../common/filterType.hpp"
#include "../dsp_math/QNumber.hpp"
#include <iostream>
#include <vector>
#include <cmath>
#include <numeric>
#include <algorithm>

namespace Filter {
class CanonicalFilter : public IFilter {
public:
    CanonicalFilter(int fs) {
        m_fs = fs;
        hu_debug(m_fs);
        m_xh1 = 0;
        m_xh2 = 0;
        m_b0 = 0;
        m_b1 = 0;
        m_b2 = 0;
        m_a1 = 0;
        m_a2 = 0;
        m_K = 0;
        m_fc = 0;
    };
    virtual void setType(int type, int fc, double factor); //fc can be f_center or f_cut based on L-HP or B-P
    int process(int input) override;
    void process(std::vector<double> &inputSignal, std::vector<int> &outputSignal) override;
    void normalize(std::vector<double> &coeff) override;

protected:
    int m_fs, m_fc;
    int m_K;
    int m_b0, m_b1, m_b2;
    int m_a1, m_a2;
    int m_xh1, m_xh2;
    double m_gain;
};
};

#endif
