#ifndef _PEAKFILTER_H_
#define _PEAKFILTER_H_

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#include "CanonicalFilter.hpp"
#include "../interface/IFilter.hpp"
#include "../common/debug.hpp"
#include "../common/filterType.hpp"
#include "../dsp_math/common.hpp"
#include "../dsp_math/QNumber.hpp"
#include <iostream>
#include <vector>
#include <cmath>
#include <numeric>
#include <algorithm>

namespace Filter {
class PeakFilter : public CanonicalFilter {
public:
    PeakFilter(int fs) : CanonicalFilter(fs) {
        m_fs = fs;
        hu_debug(m_fs);
        m_xh1 = N0_F;
        m_xh2 = N0_F;
        m_b0 = N0_F;
        m_b1 = N0_F;
        m_b2 = N0_F;
        m_a1 = N0_F;
        m_a2 = N0_F;
        m_K = N0_F;
        m_fc = N0_F;
        m_G = N0_F;
    };
    /*
        In peak filter, you can either choose canonical implement or using direct difference equations
        by setting the <type> variable. See filterType.hpp to know the id of the filter you want to use. 

        the setType allow to adjust the gain(G) and Q-factor(Q) in parameter.
    */
    virtual void setType(int type, int fc, double G, double Q); 
    void peakProcess(std::vector<double> &inputSignal, std::vector<int> &h_outputSignal);
    int peakProcess(int input);
    void setSampleFreq(int fs);

protected:
    /* m_ in Q format */
    int m_fs, m_fc;
    int m_K, m_V0;
    int m_xh1, m_xh2, m_yt1, m_xh0;
    int m_H0, m_d, m_C, m_G, m_type;
    double m_gain;
};
};

#endif
