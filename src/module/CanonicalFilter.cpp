#include "../../includes/module/CanonicalFilter.hpp"

void CanonicalFilter::setType(int type, int fc)
{
    fc = DSP_MATH::int_to_q15(fc);
    int pi_q15 = DSP_MATH::float_to_q15(M_PI);
    int Q_q15 = DSP_MATH::float_to_q15(Q);

    int pi_fc = DSP_MATH::q15_multiply(pi_q15, fc);
    int div = DSP_MATH::q15_divide(pi_fc, m_fs);
    int m_K_q15 = DSP_MATH::q15_tan(div);
    int K_square_q15 = DSP_MATH::q15_multiply(m_K_q15, m_K_q15);

    switch (type)
    {
    case LOW_PASS_1:
        m_b0 = m_K/(m_K + 1);
        m_b1 = m_K/(m_K + 1);
        m_a1 = (m_K - 1)/(m_K + 1);
        m_b2 = 0;
        m_a2 = 0;
        break;
    case HIGH_PASS_1:
        m_b0 = 1/(m_K + 1);
        m_b1 = -1/(m_K + 1);
        m_a1 = (m_K - 1)/(m_K + 1);
        break;
    case ALL_PASS_1:
        m_b0 = (m_K - 1)/(m_K + 1);
        m_b1 = 1;
        m_a1 = (m_K - 1)/(m_K + 1);
        break;
    case LOW_PASS_2:
    {
        int K2_Q_q15 = DSP_MATH::q15_multiply(K_square_q15, Q_q15);
        int det_q15 = DSP_MATH::q15_add(DSP_MATH::q15_add(K2_Q_q15, m_K_q15), Q_q15);
        
        m_b0 = DSP_MATH::q15_divide(K2_Q_q15, det_q15);
        
        int nom_b1 = DSP_MATH::q15_multiply(DSP_MATH::q15_multiply(DSP_MATH::float_to_q15(2.0), K_square_q15), Q_q15);
        m_b1 = DSP_MATH::q15_divide(nom_b1, det_q15);

        m_b2 = m_b0;
        
        int nom_a1 = DSP_MATH::q15_multiply(DSP_MATH::q15_multiply(DSP_MATH::float_to_q15(2.0), Q_q15), DSP_MATH::q15_subtract(K_square_q15, DSP_MATH::float_to_q15(1.0)));
        m_a1 = DSP_MATH::q15_divide(nom_a1, det_q15);
        
        int nom_a2 = DSP_MATH::q15_add(DSP_MATH::q15_subtract(K2_Q_q15, m_K_q15), Q_q15);
        m_a2 = DSP_MATH::q15_divide(nom_a2, det_q15);
    }
    break;

    case HIGH_PASS_2:
    {
        int K2_Q_q15 = DSP_MATH::q15_multiply(K_square_q15, Q_q15);
        int det_q15 = DSP_MATH::q15_add(DSP_MATH::q15_add(K2_Q_q15, m_K_q15), Q_q15);
        
        m_b0 = DSP_MATH::q15_divide(Q_q15, det_q15);
        m_b1 = DSP_MATH::q15_divide(DSP_MATH::q15_multiply(DSP_MATH::float_to_q15(-2.0), Q_q15), det_q15);
        m_b2 = m_b0; 

        int nom_a1 = DSP_MATH::q15_multiply(DSP_MATH::q15_multiply(DSP_MATH::float_to_q15(2.0), Q_q15), DSP_MATH::q15_subtract(K_square_q15, DSP_MATH::float_to_q15(1.0)));
        m_a1 = DSP_MATH::q15_divide(nom_a1, det_q15);
        
        int nom_a2 = DSP_MATH::q15_add(DSP_MATH::q15_subtract(K2_Q_q15, m_K_q15), Q_q15);
        m_a2 = DSP_MATH::q15_divide(nom_a2, det_q15);
    }
    break;
    case BAND_PASS_2:
        {
            int det_q15 = DSP_MATH::q15_add(DSP_MATH::q15_add(DSP_MATH::q15_multiply(K_square_q15, Q_q15), m_K_q15), Q_q15);
            
            m_b0 = DSP_MATH::q15_divide(m_K_q15, det_q15);
            
            m_b1 = 0;

            m_b2 = DSP_MATH::q15_multiply(m_b0, DSP_MATH::float_to_q15(-1.0));

            int nom_a1 = DSP_MATH::q15_multiply(DSP_MATH::q15_multiply(DSP_MATH::float_to_q15(2.0), Q_q15), DSP_MATH::q15_subtract(K_square_q15, DSP_MATH::float_to_q15(1.0)));
            m_a1 = DSP_MATH::q15_divide(nom_a1, det_q15);

            int nom_a2 = DSP_MATH::q15_add(DSP_MATH::q15_subtract(DSP_MATH::q15_multiply(K_square_q15, Q_q15), m_K_q15), Q_q15);
            m_a2 = DSP_MATH::q15_divide(nom_a2, det_q15);
        }
        break;

    case BAND_REJECT_2:
        {
            int det_q15 = DSP_MATH::q15_add(DSP_MATH::q15_add(DSP_MATH::q15_multiply(K_square_q15, Q_q15), m_K_q15), Q_q15);
            
            int nom_b0 = DSP_MATH::q15_multiply(Q_q15, DSP_MATH::q15_add(DSP_MATH::float_to_q15(1.0), K_square_q15));
            m_b0 = DSP_MATH::q15_divide(nom_b0, det_q15);

            m_b1 = DSP_MATH::q15_multiply(DSP_MATH::q15_multiply(DSP_MATH::float_to_q15(2.0), Q_q15), DSP_MATH::q15_subtract(K_square_q15, DSP_MATH::float_to_q15(1.0)));
            m_b1 = DSP_MATH::q15_divide(m_b1, det_q15);

            m_b2 = m_b0;

            int nom_a1 = DSP_MATH::q15_multiply(DSP_MATH::q15_multiply(DSP_MATH::float_to_q15(2.0), Q_q15), DSP_MATH::q15_subtract(K_square_q15, DSP_MATH::float_to_q15(1.0)));
            m_a1 = DSP_MATH::q15_divide(nom_a1, det_q15);

            int nom_a2 = DSP_MATH::q15_add(DSP_MATH::q15_subtract(DSP_MATH::q15_multiply(K_square_q15, Q_q15), m_K_q15), Q_q15);
            m_a2 = DSP_MATH::q15_divide(nom_a2, det_q15);
        }
        break;
        
    case ALL_PASS_2:
        {
            int det_q15 = DSP_MATH::q15_add(DSP_MATH::q15_add(DSP_MATH::q15_multiply(K_square_q15, Q_q15), m_K_q15), Q_q15);

            int nom_b0 = DSP_MATH::q15_add(DSP_MATH::q15_subtract(DSP_MATH::q15_multiply(K_square_q15, Q_q15), m_K_q15), Q_q15);
            m_b0 = DSP_MATH::q15_divide(nom_b0, det_q15);

            int nom_b1 = DSP_MATH::q15_multiply(DSP_MATH::q15_multiply(DSP_MATH::float_to_q15(2.0), Q_q15), DSP_MATH::q15_subtract(K_square_q15, DSP_MATH::float_to_q15(1.0)));
            m_b1 = DSP_MATH::q15_divide(nom_b1, det_q15);

            m_b2 = DSP_MATH::float_to_q15(1.0);

            int nom_a1 = DSP_MATH::q15_multiply(DSP_MATH::q15_multiply(DSP_MATH::float_to_q15(2.0), Q_q15), DSP_MATH::q15_subtract(K_square_q15, DSP_MATH::float_to_q15(1.0)));
            m_a1 = DSP_MATH::q15_divide(nom_a1, det_q15);

            int nom_a2 = DSP_MATH::q15_add(DSP_MATH::q15_subtract(DSP_MATH::q15_multiply(K_square_q15, Q_q15), m_K_q15), Q_q15);
            m_a2 = DSP_MATH::q15_divide(nom_a2, det_q15);
        }
        break;

    default:
        m_b0 = m_b1 = m_b2 = m_a1 = m_a2 = 0;
        break;
    }
}

int CanonicalFilter::process(int input)
{
    int xh0 = input - DSP_MATH::q15_multiply(m_a1, m_xh1) - DSP_MATH::q15_multiply(m_a2, m_xh2);

    int term1 = DSP_MATH::q15_multiply(m_b0, xh0);
    int term2 = DSP_MATH::q15_multiply(m_b1, m_xh1);
    int term3 = DSP_MATH::q15_multiply(m_b2, m_xh2);
    
    int y = term1 + term2 + term3;

    m_xh2 = m_xh1;
    m_xh1 = xh0;

    return y;
}

void CanonicalFilter::process(std::vector<double> &inputSignal, std::vector<int> &outputSignal)
{
    std::vector<int> processedInputSignal = DSP_MATH::preprocessQ15(inputSignal);
    for(double sample : processedInputSignal) {
        double output = this->process(sample);
        outputSignal.push_back(output);
    }
}
