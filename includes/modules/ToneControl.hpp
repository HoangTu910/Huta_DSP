#ifndef _TONECONTROL_H_
#define _TONECONTROL_H_

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#include "../interface/IFilter.hpp"
#include "../common/debug.hpp"
#include "../common/filterType.hpp"
#include "../dsp_math/QNumber.hpp"
#include "../dsp_math/common.hpp"
#include "../core/BiquadFilter.hpp"
#include "../core/PeakFilter.hpp"
#include "../core/ShelvingFilter.hpp"
#include "../interface/IDSPBlock.hpp"
#include <iostream>
#include <memory>
#include <vector>
#include <cmath>
#include <numeric>
#include <algorithm>

typedef struct {
    int freq;
    double Q; // Bass, treb don't use this
    double gain;
} ToneControlParams;

/* Tone control config */
struct TonePreset {
    ToneControlParams bass;
    ToneControlParams mid;
    ToneControlParams treble;
};

extern TonePreset mediaToneDefault;

namespace Modules {
class ToneControl : public IDSPBlock {
public:
    ToneControl(int fs) {
        m_fs = fs;
        m_trebleFilter = std::make_unique<Filter::BiquadFilter>(fs);
        m_bassFilter = std::make_unique<Filter::BiquadFilter>(fs);
        m_midFilter = std::make_unique<Filter::BiquadFilter>(fs);
    };

    void setBassFilter(int fc, double factor);
    void setTrebleFilter(int fc, double factor);
    void setMidFilter(int fc, double G, double Q);
    void setShelfSlope(int typeShelf, float shelfSlope);
    void configuration(const ToneControlParams *bass, const ToneControlParams *mid, const ToneControlParams *treble);
    void process(std::vector<double> &inputSignal) override;

private:
    int m_fs;
    int m_fcShelvingLF, m_fcShelvingHF, m_fcPeakMF1, m_fcPeakMF2;
    double m_GShelvingLF, m_GShelvingHF, m_QPeakMF1, m_QPeakMF2, m_GPeakMF1, m_GPeakMF2;
    std::unique_ptr<Filter::BiquadFilter> m_bassFilter;
    std::unique_ptr<Filter::BiquadFilter> m_midFilter;
    std::unique_ptr<Filter::BiquadFilter> m_trebleFilter;
    int process(int sample);
};
};

#endif
