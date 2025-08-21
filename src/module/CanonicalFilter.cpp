#include "../../includes/module/CanonicalFilter.hpp"

void Filter::CanonicalFilter::setType(int type, int fc, double Q)
{
    double fc_d = static_cast<double>(fc);
    double pi_d = PI_DOUBLE;
    double Q_d = Q;
    double pi_fc_d = pi_d * fc_d;
    double div_d = pi_fc_d / static_cast<double>(m_fs);
    double m_K_d = tan(div_d);
    double K_square_d = m_K_d * m_K_d;

    double b0_val, b1_val, b2_val, a1_val, a2_val;

    /*
     * All the formulas were used from DAFX-Digital Audio Effects book - Chapter 2
     * Coefficients are calculated using floating-point arithmetic for precision,
     * then converted to Q1.15 fixed-point for real-time processing.
     */

    switch (type)
    {
    case LOW_PASS_1:
        b0_val = m_K_d / (m_K_d + 1.0);
        b1_val = b0_val;
        a1_val = (m_K_d - 1.0) / (m_K_d + 1.0);
        b2_val = 0.0;
        a2_val = 0.0;
        break;
    case HIGH_PASS_1:
        b0_val = 1.0 / (m_K_d + 1.0);
        b1_val = -1.0 / (m_K_d + 1.0);
        a1_val = (m_K_d - 1.0) / (m_K_d + 1.0);
        b2_val = 0.0;
        a2_val = 0.0;
        break;
    case ALL_PASS_1:
        b0_val = (m_K_d - 1.0) / (m_K_d + 1.0);
        b1_val = 1.0;
        a1_val = (m_K_d - 1.0) / (m_K_d + 1.0);
        b2_val = 0.0;
        a2_val = 0.0;
        break;
    case LOW_PASS_2:
    {
        double K2_Q_d = K_square_d * Q_d;
        double det_d = K2_Q_d + m_K_d + Q_d;
        b0_val = K2_Q_d / det_d;
        b1_val = 2.0 * K2_Q_d / det_d;
        b2_val = b0_val;
        a1_val = 2.0 * Q_d * (K_square_d - 1.0) / det_d;
        a2_val = (K2_Q_d - m_K_d + Q_d) / det_d;
    }
    break;
    case HIGH_PASS_2:
    {
        double K2_Q_d = K_square_d * Q_d;
        double det_d = K2_Q_d + m_K_d + Q_d;
        b0_val = Q_d / det_d;
        b1_val = -2.0 * Q_d / det_d;
        b2_val = b0_val;
        a1_val = 2.0 * Q_d * (K_square_d - 1.0) / det_d;
        a2_val = (K2_Q_d - m_K_d + Q_d) / det_d;
    }
    break;
    case BAND_PASS_2:
    {
        double det_d = K_square_d * Q_d + m_K_d + Q_d;
        b0_val = m_K_d / det_d;
        b1_val = 0.0;
        b2_val = -m_K_d / det_d;
        a1_val = 2.0 * Q_d * (K_square_d - 1.0) / det_d;
        a2_val = (K_square_d * Q_d - m_K_d + Q_d) / det_d;
    }
    break;
    case BAND_REJECT_2:
    {
        double det_d = K_square_d * Q_d + m_K_d + Q_d;
        b0_val = Q_d * (1.0 + K_square_d) / det_d;
        b1_val = 2.0 * Q_d * (K_square_d - 1.0) / det_d;
        b2_val = b0_val;
        a1_val = 2.0 * Q_d * (K_square_d - 1.0) / det_d;
        a2_val = (K_square_d * Q_d - m_K_d + Q_d) / det_d;
    }
    break;
    case ALL_PASS_2:
    {
        double det_d = K_square_d * Q_d + m_K_d + Q_d;
        b0_val = (K_square_d * Q_d - m_K_d + Q_d) / det_d;
        b1_val = 2.0 * Q_d * (K_square_d - 1.0) / det_d;
        b2_val = 1.0;
        a1_val = 2.0 * Q_d * (K_square_d - 1.0) / det_d;
        a2_val = (K_square_d * Q_d - m_K_d + Q_d) / det_d;
    }
    break;
    default:
        b0_val = b1_val = b2_val = a1_val = a2_val = 0.0;
        break;
    }

    std::vector<double> coeff = {b0_val, b1_val, b2_val, a1_val, a2_val};
    this->normalize(coeff);

    hu_debug(m_a1);
    hu_debug(m_a2);
    hu_debug(m_b0);
    hu_debug(m_b1);
    hu_debug(m_b2);
    hu_debug(m_gain);
}

int Filter::CanonicalFilter::process(int input)
{
    int xh0 = DSP_MATH::q16_15_subtract(input, DSP_MATH::q16_15_add(DSP_MATH::q16_15_multiply(m_a1, m_xh1), DSP_MATH::q16_15_multiply(m_a2, m_xh2)));
    
    int term1 = DSP_MATH::q16_15_multiply(m_b0, xh0);
    int term2 = DSP_MATH::q16_15_multiply(m_b1, m_xh1);
    int term3 = DSP_MATH::q16_15_multiply(m_b2, m_xh2);
    
    int y_q = DSP_MATH::q16_15_add(DSP_MATH::q16_15_add(term1, term2), term3);
    double y = DSP_MATH::q16_15_to_float(y_q) * m_gain;
    m_xh2 = m_xh1;
    m_xh1 = xh0;

    return DSP_MATH::float_to_q16_15(y);
}

void Filter::CanonicalFilter::process(std::vector<double> &inputSignal, std::vector<int> &outputSignal)
{
    outputSignal.clear(); 
    for(double sample : inputSignal) {
    int q_sample = DSP_MATH::float_to_q16_15(sample); 
        int output = this->process(q_sample); 
        outputSignal.push_back(output); 
    }
}

void Filter::CanonicalFilter::normalize(std::vector<double>& coeff)
{
    double max_num = std::max({fabs(coeff[0]), fabs(coeff[1]), fabs(coeff[2])});
    if (max_num < 1e-12) max_num = 1.0; 

    double scale = 1.0 / max_num;
    if (scale > 1.0) scale = 1.0;

    const double headroom = 0.9999;
    if (scale * max_num > headroom && scale > 0.0) {
        scale = headroom / max_num;
    }

    m_b0 = DSP_MATH::float_to_q16_15(coeff[0] * scale);
    m_b1 = DSP_MATH::float_to_q16_15(coeff[1] * scale);
    m_b2 = DSP_MATH::float_to_q16_15(coeff[2] * scale);

    m_a1 = DSP_MATH::float_to_q16_15(coeff[3]);
    m_a2 = DSP_MATH::float_to_q16_15(coeff[4]);
    
    m_gain = (scale > 0.0) ? (1.0 / scale) : 1.0;
}

