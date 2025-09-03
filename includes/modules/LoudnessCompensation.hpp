#ifndef _LOUDNESS_COMPENSATION_H_
#define _LOUDNESS_COMPENSATION_H_ 

#include "../core/BiquadFilter.hpp"
#include "../interface/IDSPBlock.hpp"
#include <vector>
#include <memory>
#include <algorithm>

#define All(x) begin(x), end(x)

struct LoudnessPoint {
    int volumeStep;
    double bassGainDb;
    double trebleGainDb;
};

struct GainFactor {
    double bassGainDb;
    double trebleGainDb;
};

namespace Modules {

class LoudnessCompensation : public IDSPBlock {
public:
    LoudnessCompensation(int fs) {
        m_bassShelf = std::make_unique<Filter::BiquadFilter>(fs);
        m_trebleShelf = std::make_unique<Filter::BiquadFilter>(fs);
        /* Table */
        m_loudnessCurve = {
            {1, 12.0, 8.0},
            {10, 8.0, 4.0},
            {20, 3.0, 1.5},
            {30, 0.5, 0.0},
            {40, 0.0, 0.0},
        };
        m_bassFreq = BASS_LOUDNESS_COMPENSATION;
        m_trebleFreq = TREBLE_LOUDNESS_COMPENSATION;
    }

    void getGainFactor(int currentVolume);
    void configFilter(double slope, double Q);
    void process(std::vector<double>& audioBuffer) override;
private:
    std::unique_ptr<Filter::BiquadFilter> m_bassShelf;
    std::unique_ptr<Filter::BiquadFilter> m_trebleShelf;
    double m_bassFreq;
    double m_trebleFreq;
    GainFactor m_gainFactor;
    double m_bassGain, m_trebleGain;
    std::vector<LoudnessPoint> m_loudnessCurve; 
    double process(double sample);
};

}

#endif