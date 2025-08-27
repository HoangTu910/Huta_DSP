#include "../../includes/module/BiquadFilter.hpp"

void Filter::BiquadFilter::setParameters(Type type, int frequencyHz, double q, double gainDb) noexcept
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
    double b0 = N1_F, b1 = N0_F, b2 = N0_F;
    double a0 = N1_F, a1 = N0_F, a2 = N0_F;

    const double A = std::pow(N10_F, m_gaindB / N40_F);
    const double V = std::pow(N10_F, m_gaindB / N20_F);

    hu_debug(m_fc/m_fs);
    hu_debug(m_shelfSlope);
    double omega = N2_F * PI_DOUBLE * ((m_fc + N0_F)/(m_fs + N0_F));
    double sinOmega = std::sin(omega);
    double cosOmega = std::cos(omega);
    double alphaQ = sinOmega / (N2_F * m_q);
    double alphaBW = sinOmega * std::sinh((log(N2_F * N1_F) / N2_F) * m_q * ((omega + N0_F) / (sinOmega + N0_F)));
    double alphaS = (sinOmega / N2_F) * std::sqrt((A + N1_F / A) * (N1_F / m_shelfSlope - N1_F) + N2_F);

    hu_debug(omega);
    hu_debug(cosOmega);
    hu_debug(sinOmega);
    hu_debug(alphaS);

    switch(m_type) {
        case Type::LowPass:
            b0 = (N1_F - cosOmega) / N2_F;
            b1 = N1_F - cosOmega;
            b2 = (N1_F - cosOmega) / N2_F;
            a0 = N1_F + alphaQ;
            a1 = -N2_F * cosOmega;
            a2 = N1_F - alphaQ;
            break;
        case Type::HighPass:
            b0 = (N1_F + cosOmega) / N2_F;
            b1 = -(N1_F + cosOmega);
            b2 = (N1_F + cosOmega) / N2_F;
            a0 = N1_F + alphaQ;
            a1 = -N2_F * cosOmega;
            a2 = N1_F - alphaQ;
            break;
        case Type::BandPass_Q:
            b0 = m_q * alphaQ;
            b1 = N0_F;
            b2 = -m_q * alphaQ;
            a0 = N1_F + alphaQ;
            a1 = -N2_F * cosOmega;
            a2 = N1_F - alphaQ;
            break;
        case Type::BandPass_Constant:
            b0 = alphaQ;
            b1 = N0_F;
            b2 = -alphaQ;
            a0 = N1_F + alphaQ;
            a1 = -N2_F * cosOmega;
            a2 = N1_F - alphaQ;
            break;
        case Type::Notch:
            b0 = N1_F;
            b1 = -N2_F * cosOmega;
            b2 = N1_F;
            a0 = N1_F + alphaQ;
            a1 = -N2_F * cosOmega;
            a2 = N1_F - alphaQ;
            break;
        case Type::AllPass:
            b0 = N1_F - alphaQ;
            b1 = -N2_F * cosOmega;
            b2 = N1_F + alphaQ;
            a0 = N1_F + alphaQ;
            a1 = -N2_F * cosOmega;
            a2 = N1_F - alphaQ;
            break;
        case Type::Peaking:
            b0 = N1_F + alphaQ * A;
            b1 = -N2_F * cosOmega;
            b2 = N1_F - alphaQ * A;
            a0 = N1_F + alphaQ / A;
            a1 = -N2_F * cosOmega;
            a2 = N1_F - alphaQ / A;
            break;
        case Type::LowShelf:
            b0 = A * ((A + N1_F) - (A - N1_F) * cosOmega + N2_F * std::sqrt(A) * alphaS);
            b1 = N2_F * A * ((A - N1_F) - (A + N1_F) * cosOmega);
            b2 = A * ((A + N1_F) - (A - N1_F) * cosOmega - N2_F * std::sqrt(A) * alphaS);
            a0 = (A + N1_F) + (A - N1_F) * cosOmega + N2_F * std::sqrt(A) * alphaS;
            a1 = -N2_F * ((A - N1_F) + (A + N1_F) * cosOmega);
            a2 = (A + N1_F) + (A - N1_F) * cosOmega - N2_F * std::sqrt(A) * alphaS;
            break;
        case Type::HighShelf:
            b0 = A * ((A + N1_F) + (A - N1_F) * cosOmega + N2_F * std::sqrt(A) * alphaS);
            b1 = -N2_F * A * ((A - N1_F) + (A + N1_F) * cosOmega);
            b2 = A * ((A + N1_F) + (A - N1_F) * cosOmega - N2_F * std::sqrt(A) * alphaS);
            a0 = (A + N1_F) - (A - N1_F) * cosOmega + N2_F * std::sqrt(A) * alphaS;
            a1 = N2_F * ((A - N1_F) - (A + N1_F) * cosOmega);
            a2 = (A + N1_F) - (A - N1_F) * cosOmega - N2_F * std::sqrt(A) * alphaS;
            break;
        default:
            b0 = N1_F, b1 = N0_F, b2 = N0_F;
            a0 = N1_F, a1 = N0_F, a2 = N0_F;
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
// Parameter accessors
double Filter::BiquadFilter::getFrequencyHz() const { return m_fc; }
void Filter::BiquadFilter::setFrequencyHz(int frequencyHz) { m_fc = frequencyHz; recalculateCoefficients(); }
double Filter::BiquadFilter::getQ() const { return m_q; }
void Filter::BiquadFilter::setQ(double q) { m_q = q; recalculateCoefficients(); }
double Filter::BiquadFilter::getGainDb() const { return m_gaindB; }
void Filter::BiquadFilter::setGainDb(double gainDb) { m_gaindB = gainDb; recalculateCoefficients(); }

