#include "../../includes/module/BiquadFilter.hpp"

void Filter::BiquadFilter::setParameters(Type type, double frequencyHz, double q, double gainDb) noexcept
{
    m_type = type;
    m_fc = frequencyHz;
    m_q = q;
    m_gaindB = gainDb;
    recalculateCoefficients();
}

void Filter::BiquadFilter::setShelfSlope(double shelfSlope)
{
    m_shelfSlope = shelfSlope;
}

void Filter::BiquadFilter::recalculateCoefficients()
{
    /*
        Pass-through coefficient
    */
    double b0 = 1.0, b1 = 0.0, b2 = 0.0;
    double a0 = 1.0, a1 = 0.0, a2 = 0.0;

    const double A = std::pow(10, m_gaindB / 40.0);
    const double V = std::pow(10.0, m_gaindB / 20.0);

    hu_debug(m_fc/m_fs);
    
    double omega = 2 * PI_DOUBLE * ((m_fc + 0.0)/(m_fs + 0.0));
    double sinOmega = std::sin(omega);
    double cosOmega = std::cos(omega);
    double alphaQ = sinOmega / (2.0 * m_q);
    double alphaBW = sinOmega * std::sinh((log(2) / 2.0) * m_q * ((omega + 0.0) / (sinOmega + 0.0)));
    double alphaS = (sinOmega / 2.0) * std::sqrt((A + 1.0 / A) * (1.0 / m_shelfSlope - 1) + 2);

    hu_debug(omega);
    hu_debug(cosOmega);
    hu_debug(sinOmega);

    switch(m_type) {
        case Type::LowPass:
            b0 = (1.0 - cosOmega) / 2.0;
            b1 = 1.0 - cosOmega;
            b2 = (1.0 - cosOmega) / 2.0;
            a0 = 1.0 + alphaQ;
            a1 = -2.0 * cosOmega;
            a2 = 1.0 - alphaQ;
            break;
        case Type::HighPass:
            b0 = (1.0 + cosOmega) / 2.0;
            b1 = -(1.0 + cosOmega);
            b2 = (1.0 + cosOmega) / 2.0;
            a0 = 1.0 + alphaQ;
            a1 = -2.0 * cosOmega;
            a2 = 1.0 - alphaQ;
            break;
        case Type::BandPass_Q:
            b0 = m_q * alphaQ;
            b1 = 0.0;
            b2 = -m_q * alphaQ;
            a0 = 1.0 + alphaQ;
            a1 = -2 * cosOmega;
            a2 = 1.0 - alphaQ;
            break;
        case Type::BandPass_Constant:
            b0 = alphaQ;
            b1 = 0.0;
            b2 = -alphaQ;
            a0 = 1.0 + alphaQ;
            a1 = -2.0 * cosOmega;
            a2 = 1.0 - alphaQ;
            break;
        case Type::Notch:
            b0 = 1.0;
            b1 = -2.0 * cosOmega;
            b2 = 1.0;
            a0 = 1.0 + alphaQ;
            a1 = -2.0 * cosOmega;
            a2 = 1.0 - alphaQ;
            break;
        case Type::AllPass:
            b0 = 1.0 - alphaQ;
            b1 = -2.0 * cosOmega;
            b2 = 1.0 + alphaQ;
            a0 = 1.0 + alphaQ;
            a1 = -2.0 * cosOmega;
            a2 = 1.0 - alphaQ;
            break;
        case Type::Peaking:
            b0 = 1.0 + alphaQ * A;
            b1 = -2.0 * cosOmega;
            b2 = 1.0 - alphaQ * A;
            a0 = 1.0 + alphaQ / A;
            a1 = -2.0 * cosOmega;
            a2 = 1.0 - alphaQ / A;
            break;
        case Type::LowShelf:
            b0 = A * ((A + 1.0) - (A - 1.0) * cosOmega + 2.0 * std::sqrt(A) * alphaS);
            b1 = 2.0 * A * ((A - 1.0) - (A + 1.0) * cosOmega);
            b2 = A * ((A + 1.0) - (A - 1.0) * cosOmega - 2.0 * std::sqrt(A) * alphaS);
            a0 = (A + 1.0) + (A - 1.0) * cosOmega + 2.0 * std::sqrt(A) * alphaS;
            a1 = -2.0 * ((A - 1.0) + (A + 1.0) * cosOmega);
            a2 = (A + 1.0) + (A - 1.0) * cosOmega - 2.0 * std::sqrt(A) * alphaS;
            break;
        case Type::HighShelf:
            b0 = A * ((A + 1.0) + (A - 1.0) * cosOmega + 2.0 * std::sqrt(A) * alphaS);
            b1 = -2.0 * A * ((A - 1.0) + (A + 1.0) * cosOmega);
            b2 = A * ((A + 1.0) + (A - 1.0) * cosOmega - 2.0 * std::sqrt(A) * alphaS);
            a0 = (A + 1.0) - (A - 1.0) * cosOmega + 2.0 * std::sqrt(A) * alphaS;
            a1 = 2.0 * ((A - 1.0) - (A + 1.0) * cosOmega);
            a2 = (A + 1.0) - (A - 1.0) * cosOmega - 2.0 * std::sqrt(A) * alphaS;
            break;
        default:
            b0 = 1.0, b1 = 0.0, b2 = 0.0;
            a0 = 1.0, a1 = 0.0, a2 = 0.0;
            break;
    }
    hu_debug2(b0, a0);
    hu_debug(b0 / a0);
    std::vector<double> coeff = {b0 / a0, b1 / a0, b2 / a0, a1 / a0, a2 / a0};
    this->normalize(coeff);

    hu_debug(m_a1);
    hu_debug(m_a2);
    hu_debug(m_b0);
    hu_debug(m_b1);
    hu_debug(m_b2);
    hu_debug(m_gain);
}
