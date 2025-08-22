#include "../../includes/module/PeakFilter.hpp"

void Filter::PeakFilter::setType(int type, int fc, double G, double Q) {
    m_type = type;

    if((m_type == PEAK_BOOST_MODE && G < 0) || (m_type == PEAK_CUT_MODE && G > 0)) {
        hu_alert("Consider changing for gain in boost mode!");
    }

    double G_d = G;
    double V0_d = pow(10.0, G_d / 20.0);
    double d_d = -cos((2 * PI_DOUBLE * fc) / static_cast<double>(m_fs));
    double H0_d = V0_d - 1;
    double cB_d = (tan((M_PI * fc) / static_cast<double>(m_fs)) - 1)/(tan((M_PI * fc) / static_cast<double>(m_fs)) + 1);
    double cC_d = (tan((M_PI * fc) / static_cast<double>(m_fs)) - V0_d)/(tan((M_PI * fc) / static_cast<double>(m_fs)) + V0_d);
    double K_d = tan((M_PI * fc) / static_cast<double>(m_fs));
    double K_square_d = K_d * K_d;
    double b0_val = 0.0, b1_val = 0.0, b2_val = 0.0, a1_val = 0.0, a2_val = 0.0;
    double det = 0.0;

    switch(type) {
        case CANONICAL_BOOST_MODE: {
            det = 1.0 + (1.0 / Q) * K_d + K_square_d;
            b0_val = (1.0 + (V0_d / Q) * K_d + K_square_d) / det;
            b1_val = (2.0 * (K_square_d - 1.0)) / det;
            b2_val = (1.0 - (V0_d / Q) * K_d + K_square_d) / det;
            a1_val = (2.0 * (K_square_d - 1.0)) / det;
            a2_val = (1.0 - (1.0 / Q) * K_d + K_square_d) / det;
            break;
        }
        case CANONICAL_CUT_MODE: {
            det = 1.0 + (1.0 / (V0_d * Q)) * K_d + K_square_d;
            b0_val = (1.0 + (1.0 / Q) * K_d + K_square_d) / det;
            b1_val = (2.0 * (K_square_d - 1.0)) / det;
            b2_val = (1.0 - (1.0 / Q) * K_d + K_square_d) / det;
            a1_val = (2.0 * (K_square_d - 1.0)) / det;
            a2_val = (1.0 - (1.0 / (V0_d * Q)) * K_d + K_square_d) / det;
            break;
        }
        case PEAK_BOOST_MODE: {
            m_C = DSP_MATH::float_to_q16_15(cB_d);
            break;
        }
        case PEAK_CUT_MODE: {
            m_C = DSP_MATH::float_to_q16_15(cB_d);
            break;
        }
        default:
            b0_val = b1_val = b2_val = a1_val = a2_val = 0;
            break;
    }

    std::vector<double> coeff = {b0_val, b1_val, b2_val, a1_val, a2_val};
    this->normalize(coeff);

    m_b0 = DSP_MATH::float_to_q16_15(b0_val);
    m_b1 = DSP_MATH::float_to_q16_15(b1_val);
    m_b2 = DSP_MATH::float_to_q16_15(b2_val);
    m_a1 = DSP_MATH::float_to_q16_15(a1_val);
    m_a2 = DSP_MATH::float_to_q16_15(a2_val);
    m_H0 = DSP_MATH::float_to_q16_15(H0_d);
    m_d = DSP_MATH::float_to_q16_15(d_d);

    hu_debug(m_b0);
    hu_debug(m_b1);
    hu_debug(m_b2);
    hu_debug(m_a1);
    hu_debug(m_a2);
    hu_debug(m_H0);
    hu_debug(m_d);
}

int Filter::PeakFilter::peakProcess(int input) {
    int one_q = DSP_MATH::float_to_q16_15(1.0);
    
    // m_xh0 = input - m_d * (1 - m_C) * m_xh1 + m_C * m_xh2
    int term1_sub = DSP_MATH::q16_15_subtract(one_q, m_C); 
    int term1_mul1 = DSP_MATH::q16_15_multiply(m_d, term1_sub); 
    int term1_mul2 = DSP_MATH::q16_15_multiply(term1_mul1, m_xh1); 
    int term1_add = DSP_MATH::q16_15_add(input, DSP_MATH::q16_15_multiply(m_C, m_xh2)); 
    m_xh0 = DSP_MATH::q16_15_subtract(term1_add, term1_mul2);

    // m_yt1 = -m_C * m_xh0 + m_d * (1 - m_C) * m_xh1 + m_xh2
    int neg_m_C = DSP_MATH::q16_15_multiply(m_C, DSP_MATH::float_to_q16_15(-1.0));
    int term2_mul1 = DSP_MATH::q16_15_multiply(neg_m_C, m_xh0); 
    int term2_add1 = DSP_MATH::q16_15_add(term2_mul1, term1_mul2); 
    m_yt1 = DSP_MATH::q16_15_add(term2_add1, m_xh2); 

    // y = (m_H0/2)*(input - m_yt1) + input
    int H0_div_2 = DSP_MATH::q16_15_multiply(m_H0, DSP_MATH::float_to_q16_15(0.5)); 
    int diff = DSP_MATH::q16_15_subtract(input, m_yt1); 
    int mul = DSP_MATH::q16_15_multiply(H0_div_2, diff); 
    int y = DSP_MATH::q16_15_add(mul, input); 
    
    m_xh2 = m_xh1;
    m_xh1 = m_xh0;

    return y; 
}

void Filter::PeakFilter::peakProcess(std::vector<double> &inputSignal, std::vector<int> &h_outputSignal)
{
    h_outputSignal.clear(); 
    if(m_type == CANONICAL_BOOST_MODE || m_type == CANONICAL_CUT_MODE) {
        process(inputSignal, h_outputSignal);
    }
    else if(m_type == PEAK_BOOST_MODE || m_type == PEAK_CUT_MODE){
        for(double sample : inputSignal) {
        int q_sample = DSP_MATH::float_to_q16_15(sample); 
            int output = this->peakProcess(q_sample); 
            h_outputSignal.push_back(output); 
        }
    }
    else {
        hu_alert("It still work but may be wrong co-efficient!");
    }
}