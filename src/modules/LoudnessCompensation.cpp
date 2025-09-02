#include "../includes/modules/LoudnessCompensation.hpp"

void Modules::LoudnessCompensation::getGainFactor(int currentVolume)
{
    /* Find first element with volumeStep >= currentVolume */
    auto x2 = std::lower_bound(
        m_loudnessCurve.begin(),
        m_loudnessCurve.end(),
        currentVolume,
        [](const LoudnessPoint& point, int value) {
            return point.volumeStep < value;
        }
    );

    if(x2 == m_loudnessCurve.begin()) {
        m_bassGain = x2->bassGainDb;
        m_trebleGain = x2->trebleGainDb;
        return;
    }
    if(x2 == m_loudnessCurve.end()) {
        m_bassGain = x2->bassGainDb;
        m_trebleGain = x2->trebleGainDb;
        return;
    }

    auto x1 = std::prev(x2);
    double fraction = double(currentVolume - x1->volumeStep + 0.0) / 
                      double(x2->volumeStep - x1->volumeStep + 0.0);
    
    double bassGain = x1->bassGainDb + fraction * (x2->bassGainDb - x1->bassGainDb);
    double trebleGain = x1->trebleGainDb + fraction * (x2->trebleGainDb - x1->trebleGainDb);
}

void Modules::LoudnessCompensation::configFilter(double slope, double Q)
{
    m_bassShelf->setShelfSlope(slope);
    m_trebleShelf->setShelfSlope(slope);
    m_bassShelf->setParameters(Type::LowShelf, m_bassFreq, Q, m_bassGain);
    m_trebleShelf->setParameters(Type::HighShelf, m_trebleFreq, Q, m_trebleGain);
}

double Modules::LoudnessCompensation::process(double sample)
{
    double sampleAfterBass = m_bassShelf->process(sample);
    double outputSample = m_trebleShelf->process(sampleAfterBass);
    
    return outputSample;
}

void Modules::LoudnessCompensation::process(std::vector<double> &audioBuffer)
{
    for(size_t i = 0; i < audioBuffer.size(); i++) {
        int q_sample = DSP_MATH::float_to_q16_15(audioBuffer[i]);
        audioBuffer[i] = process(q_sample);
    }
}