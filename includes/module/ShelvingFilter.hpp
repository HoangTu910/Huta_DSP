#ifndef _SHELVINGFILTER_H_
#define _SHELVINGFILTER_H_

#include "CanonicalFilter.hpp"
#include "../common/debug.hpp"
#include "../common/filterType.hpp"
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
        m_fs = DSP_MATH::int_to_q17_14(fs);
        m_xh1 = DSP_MATH::q17_14_to_int(0);
        m_xh2 = DSP_MATH::q17_14_to_int(0);
        m_b0 = DSP_MATH::q17_14_to_int(0);
        m_b1 = DSP_MATH::q17_14_to_int(0);
        m_b2 = DSP_MATH::q17_14_to_int(0);
        m_a1 = DSP_MATH::q17_14_to_int(0);
        m_a2 = DSP_MATH::q17_14_to_int(0);
        m_K = DSP_MATH::q17_14_to_int(0);
        m_fc = DSP_MATH::q17_14_to_int(0);
    }
    void setType(int type, int fc, double factor) override; //G is gain instead of Q
};
}

#endif