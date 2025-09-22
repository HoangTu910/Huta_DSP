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
    if(m_mode == FaderType::Linear) {
        double maxStep = m_slopeLinear / SAMP_FREQ;
        double diff = m_targetGain - m_currentGain;
        if(std::abs(diff) < maxStep) m_currentGain = m_targetGain;
        else m_currentGain += (diff > 0 ? 1 : -1) * maxStep;
    }
    else {
        double maxStep = m_slopeExp / SAMP_FREQ;
        if(m_currentGain < 1e-6) m_currentGain = 1e-6; // avoid log(0)
        double gainDb = 20.0 * log10(m_currentGain);
        double diffDb = 20.0 * log10(m_targetGain) - gainDb;
        if(std::abs(diffDb) < maxStep) gainDb += diffDb;
        else gainDb += (diffDb > 0 ? 1 : -1) * maxStep;
        m_currentGain = pow(10.0, gainDb / 20.0);
    }
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
