#ifndef _FADER_H_
#define _FADER_H_

#include "../interface/IDSPBlock.hpp"
#include "../dsp_math/QNumber.hpp"
#include "../dsp_math/common.hpp"
#include "../common/filterType.hpp"

namespace Modules
{
class Fader : public IDSPBlock{
public:
    Fader(double slopeLinear, double slopeExp, FaderType type, double target) 
        : m_slopeLinear(slopeLinear), m_slopeExp(slopeExp), m_mode(type)
    {
        m_step = 0.0;
        m_remainingSamples = 0;

        m_targetGain = std::min(0.9, std::max(0.1, target)); // clamp target between 0.0 and 0.9

        /* Min gain dB */
        double minGainDb = -80.0;
        m_currentGain = pow(10.0, minGainDb / 20.0); // start from very low gain

        /* Calculate step for linear slope */
        double fadeTimeSec = m_slopeLinear / 1000.0;
        m_remainingSamples = static_cast<int>(fadeTimeSec * SAMP_FREQ);
        if (m_remainingSamples < 1) m_remainingSamples = 1;
        m_step = (m_targetGain - m_currentGain) / m_remainingSamples;
    }

    void setTarget(double target);
    void mute();
    void unmute(double volume);
    double process(double sample);
    void process(std::vector<double>& buffer) override;

private:
    double m_currentGain;
    double m_targetGain;
    double m_slopeLinear;
    double m_slopeExp;
    double m_step;
    int m_remainingSamples;
    FaderType m_mode;
    void vCalculateGain();
    void vCalculateLinearGain();
    void vCalculateExponentialGain();
};
} 


#endif