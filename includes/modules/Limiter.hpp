#ifndef _LIMITER_HPP_
#define _LIMITER_HPP_

#include "../interface/IDSPBlock.hpp"
#include <cmath>

#define LIMITER_AT 0.0001f
#define LIMITER_RT 0.0001f
#define LIMITER_TH 3 // 3dB
#define LIMITER_DELAY_SAMPLES 5

namespace Modules {
class Limiter : public IDSPBlock {
public:
    Limiter(double thresholdDb, double attackTimeMs, double releaseTimeMs, int delaySamples) 
        :   m_thresholdDb(thresholdDb),
            m_attackTimeMs(attackTimeMs),
            m_releaseTimeMs(releaseTimeMs),
            m_delaySamples(delaySamples),
            m_buffer(delaySamples, 0.0)
    {
        m_envelope = 0.0;
        m_gain = 1.0;
        m_delayIndex = 0;
    }

    void process(std::vector<double>& buffer) override;
    double process(double sample);

private:
    double m_thresholdDb;
    double m_attackTimeMs;
    double m_releaseTimeMs;
    int m_delaySamples;
    double m_envelope;
    double m_gain;
    std::vector<double> m_buffer;
    int m_delayIndex;
};
}

#endif