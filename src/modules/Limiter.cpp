#include "../includes/modules/Limiter.hpp"

void Limiter::process(std::vector<double>& buffer) {
    for (auto& sample : buffer) {
        process(sample);
    }
}

double Limiter::process(double sample) {
    /* Peak rectifier */
    const double xPeak = std::abs(sample);

    /* Envelope follower with Attack/Release times */
    const double coeffAttack = (xPeak > m_envelope) ? m_attackTimeMs : m_releaseTimeMs;
    m_envelope = (1.0 - coeffAttack) * m_envelope + coeffAttack * xPeak;

    /* Gain target */
    constexpr double eps = 1e-30;
    const double denom = (m_envelope > eps) ? m_envelope : eps;
    const double f = std::min(1.0, m_thresholdDb / denom);

    /* Smoothing */
    m_gain = (1.0 - coeffAttack) * m_gain + coeffAttack * f;

    /* Delay */
    double delayed = (m_delaySamples > 0) ? m_buffer[m_delayIndex] : sample;
    if(m_delaySamples > 0) {
        m_buffer[m_delayIndex] = sample;
        m_delayIndex = (m_delayIndex + 1) % m_delaySamples;
    }

    return delayed * m_gain;
}
