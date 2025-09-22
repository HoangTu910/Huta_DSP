#include "../includes/modules/Limiter.hpp"

void Modules::Limiter::process(std::vector<double>& buffer) {
    hu_alert("Modules::Limiter processing");
    for (auto& sample : buffer) {
        // TODO: Need to process in Q-format
        // int q_sample = DSP_MATH::float_to_q16_15(sample);
        sample = process(sample);
    }
}

int cntLim = 0;

double Modules::Limiter::process(double sample) {
    double k = 0.0;
    double thresholdLinear = std::pow(10.0, m_thresholdDb / 20.0);

    /* Peak rectifier */
    const double xPeak = std::abs(sample);
    bool isAttack = (xPeak > m_envelope);

    /* Envelope follower with Attack/Release times */
    if(isAttack) {
        m_envelope = (1.0 - m_attackCoefficient) * m_envelope + m_attackCoefficient * xPeak;
        k = m_attackCoefficient; 
    }
    else {
        m_envelope = (1.0 - m_releaseCoefficient) * m_envelope + m_releaseCoefficient * xPeak;
        k = m_releaseCoefficient;
    }
    
    /* AT/RT */
    double f = std::min(1.0, (thresholdLinear/(m_envelope + 0.0)));
    if(f < m_gain) {
        k = m_attackCoefficient;
    }
    else {
        k = m_releaseCoefficient;
    }
    /* Smoothing filter */
    m_gain = (1.0 - k) * m_gain + k * f;

    /* Explanation: Both peak measurement and smoothing filter using the same AT/RT coefficient */

    if(cntLim > 50000 && cntLim < 50006) {
        hu_debug(sample);
        hu_debug(m_envelope);
        hu_debug(m_gain);
        hu_debug(k);
        hu_debug(f);
        hu_debug(thresholdLinear);
        hu_debug(m_delaySamples);
    }
    cntLim++;


    /* Delay */
    double delayed = (m_delaySamples > 0) ? m_buffer[m_delayIndex] : sample;
    if(m_delaySamples > 0) {
        m_buffer[m_delayIndex] = sample;
        m_delayIndex = (m_delayIndex + 1) % m_delaySamples;
    }

    return delayed * m_gain;
}
