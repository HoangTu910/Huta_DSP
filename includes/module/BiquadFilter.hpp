#ifndef _BIQUAD_FILTER_H_
#define _BIQUAD_FILTER_H_

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#include "../interface/IFilter.hpp"
#include "../common/debug.hpp"
#include "../common/filterType.hpp"
#include "../dsp_math/QNumber.hpp"
#include "../dsp_math/common.hpp"
#include "CanonicalFilter.hpp"
#include <iostream>
#include <vector>
#include <cmath>
#include <numeric>
#include <algorithm>

namespace Filter {
class BiquadFilter : public CanonicalFilter {
public:
    BiquadFilter(int fs) : CanonicalFilter(fs) {
        m_fs = fs;
        m_shelfSlope = N1_F; // standard
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

    /**
     * @brief the double q can be Q or BW or S
        Q: Q factor
        BW: Bandwidth
        S: shelf slope
        @attention: the gainDb only use for peak and shelving filter, if you use other type, you can set gainDb whatever you want
     */
    void setParameters(Type type, int frequencyHz, double q, double gainDb) noexcept;
    void setShelfSlope(double shelfSlope);
    
    // Parameter accessors
    double getFrequencyHz() const;
    void setFrequencyHz(int frequencyHz);
    double getQ() const;
    void setQ(double q);
    double getGainDb() const;
    void setGainDb(double gainDb);

private:
    double m_gaindB {N0_F}, m_shelfSlope;
    double m_q {COMMON_Q_FACTOR};
    Type m_type {Type::LowPass};
    void recalculateCoefficients();
};
};

#endif
