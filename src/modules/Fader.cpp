#include "../includes/modules/Fader.hpp"

void Modules::Fader::setTarget(double target)
{
    m_targetGain = target;
}

void Modules::Fader::mute()
{
    setTarget(0.0);
}

void Modules::Fader::unmute(double volume)
{
    setTarget(volume);
}

void Modules::Fader::vCalculateGain()
{
    switch (m_mode) {
        case FaderType::Linear:
            vCalculateLinearGain();
            break;
        case FaderType::Exponential:
            vCalculateExponentialGain();
            break;
        case FaderType::Hybrid:
            if(m_currentGain < 0.1 /* - 20 dB */) {
                vCalculateLinearGain();
            } else {
                vCalculateExponentialGain();
            }
            break;
        default:
            hu_alert("Modules::Fader unknown FaderType");
            break;
    }
}

void Modules::Fader::vCalculateLinearGain()
{
    if(m_remainingSamples > 0) {
        m_currentGain += m_step;
        m_remainingSamples--;
    } else {
        m_currentGain = m_targetGain;
    }
}

void Modules::Fader::vCalculateExponentialGain()
{
    double stepDb = m_slopeExp / SAMP_FREQ;
    if(m_currentGain < 1e-6) m_currentGain = 1e-6; // avoid log(0)
    double gainDb = 20.0 * log10(m_currentGain);
    double diffDb = 20.0 * log10(m_targetGain) - gainDb;
    if(std::abs(diffDb) < stepDb) gainDb += diffDb;
    else gainDb += (diffDb > 0 ? 1 : -1) * stepDb;
    m_currentGain = pow(10.0, gainDb / 20.0);
}

double Modules::Fader::process(double sample)
{
    vCalculateGain();
    return sample * m_currentGain;
}

void Modules::Fader::process(std::vector<double> &buffer)
{
    hu_alert("Modules::Fader processing");
    for (auto& sample : buffer) {
        // TODO: Need to process in Q-format
        // int q_sample = DSP_MATH::float_to_q16_15(sample);
        sample = process(sample);
    }
}
