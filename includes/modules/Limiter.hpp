#ifndef _LIMITER_HPP_
#define _LIMITER_HPP_

#include "../interface/IDSPBlock.hpp"
#include "../dsp_math/QNumber.hpp"
#include <cmath>

#define LIMITER_AT 0.0001f
#define LIMITER_RT 0.0001f
#define LIMITER_TH 3 // 3dB
#define LIMITER_DELAY_SAMPLES 5
#define Ts 0.00002083333

namespace Modules {
class Limiter : public IDSPBlock {
public:
    Limiter(double thresholdDb, double attackTimeMs, double releaseTimeMs, int delaySamples) 
        :   m_thresholdDb(thresholdDb),
            m_attackTimeMs(attackTimeMs),
            m_releaseTimeMs(releaseTimeMs),
            m_delaySamples(static_cast<int>(delaySamples * (48000.0 / 1000.0))),
            m_buffer(static_cast<int>(delaySamples * (48000.0 / 1000.0)), 0.0)
    {
        hu_alert("Modules::Limiter activated");
        m_envelope = 1e-10;
        m_gain = 1.0;
        m_delayIndex = 0;
        m_attackCoefficient = 1 - exp((-2.2 * Ts) / (m_attackTimeMs / 1000.0));
        m_releaseCoefficient = 1 - exp((-2.2 * Ts) / (m_releaseTimeMs / 1000.0));
        m_limiterSlope = -1.5;
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
    double m_attackCoefficient;
    double m_releaseCoefficient;
    double m_limiterSlope;
    std::vector<double> m_buffer;
    int m_delayIndex;
};
}

#endif