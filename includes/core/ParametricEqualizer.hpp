#ifndef _EQUALIZER_H_
#define _EQUALIZER_H_

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#include "../interface/IFilter.hpp"
#include "../common/debug.hpp"
#include "../common/filterType.hpp"
#include "../dsp_math/QNumber.hpp"
#include "../dsp_math/common.hpp"
#include "../core/BiquadFilter.hpp"
#include "PeakFilter.hpp"
#include "ShelvingFilter.hpp"
#include <iostream>
#include <memory>
#include <vector>
#include <cmath>
#include <numeric>
#include <algorithm>

namespace Equalizer {
class ParametricEqualizer {
public:
    ParametricEqualizer(int fs) {
        m_fs = fs;
        m_firstMidPeak = std::make_unique<Filter::BiquadFilter>(fs);
        m_secondMidPeak = std::make_unique<Filter::BiquadFilter>(fs);
        m_lowShelf = std::make_unique<Filter::BiquadFilter>(fs);
        m_highShelf = std::make_unique<Filter::BiquadFilter>(fs);
    };

    void setLowShelf(int fc, double factor);
    void setHighShelf(int fc, double factor);
    void setFirstMidPeak(int fc, double G, double Q);
    void setSecondMidPeak(int fc, double G, double Q);
    void setShelfSlope(int typeShelf, float shelfSlope);
    std::vector<int> process(std::vector<double> &inputSignal);

private:
    int m_fs;
    int m_fcShelvingLF, m_fcShelvingHF, m_fcPeakMF1, m_fcPeakMF2;
    double m_GShelvingLF, m_GShelvingHF, m_QPeakMF1, m_QPeakMF2, m_GPeakMF1, m_GPeakMF2;
    std::unique_ptr<Filter::BiquadFilter> m_firstMidPeak;
    std::unique_ptr<Filter::BiquadFilter> m_secondMidPeak;
    std::unique_ptr<Filter::BiquadFilter> m_lowShelf;
    std::unique_ptr<Filter::BiquadFilter> m_highShelf;
    int process(int sample);
};
};

#endif
