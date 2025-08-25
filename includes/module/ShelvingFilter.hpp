#ifndef _SHELVINGFILTER_H_
#define _SHELVINGFILTER_H_

#include "CanonicalFilter.hpp"
#include "../common/debug.hpp"
#include "../common/filterType.hpp"
#include "../dsp_math/common.hpp"
#include "../dsp_math/QNumber.hpp"
#include <iostream>
#include <vector>
#include <cmath>
#include <numeric>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

/* For shelving filter, I only implement second-order because it mostly used for Equalizer */
namespace Filter {
class ShelvingFilter : public CanonicalFilter {
public:
    ShelvingFilter(int fs) : CanonicalFilter(fs) {
        m_fs = fs;
        m_xh1 = N0_F;
        m_xh2 = N0_F;
        m_b0 = N0_F;
        m_b1 = N0_F;
        m_b2 = N0_F;
        m_a1 = N0_F;
        m_a2 = N0_F;
        m_K = N0_F;
        m_fc = N0_F;
    }
    void setType(int type, int fc, double factor) override; //G is gain instead of Q
    void setSampleFreq(int fs);
};
}

#endif