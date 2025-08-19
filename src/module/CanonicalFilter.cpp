#include "../../includes/module/CanonicalFilter.hpp"

void Filter::CanonicalFilter::setType(int type, int fc, double Q)
{
    int fc_q = DSP_MATH::int_to_q17_14(fc);
    hu_debug(fc_q);
    int pi_q = DSP_MATH::float_to_q17_14(M_PI);
    hu_debug(pi_q);
    int Q_q = DSP_MATH::float_to_q17_14(Q);
    hu_debug(Q_q);
    int pi_fc = DSP_MATH::q17_14_multiply(pi_q, fc_q);
    hu_debug(pi_fc);
    int div = DSP_MATH::q17_14_divide(pi_fc, m_fs);
    hu_debug(div);
    int m_K_q = DSP_MATH::q17_14_tan(div);
    hu_debug(m_K_q);
    int K_square_q = DSP_MATH::q17_14_multiply(m_K_q, m_K_q);

    switch (type)
    {
    case LOW_PASS_1:
        m_b0 = DSP_MATH::q17_14_divide(m_K_q, DSP_MATH::q17_14_add(m_K_q, DSP_MATH::float_to_q17_14(1.0)));
        m_b1 = m_b0;
        m_a1 = DSP_MATH::q17_14_divide(DSP_MATH::q17_14_subtract(m_K_q, DSP_MATH::float_to_q17_14(1.0)), DSP_MATH::q17_14_add(m_K_q, DSP_MATH::float_to_q17_14(1.0)));
        m_b2 = 0;
        m_a2 = 0;
        break;
    case HIGH_PASS_1:
        m_b0 = DSP_MATH::q17_14_divide(DSP_MATH::float_to_q17_14(1.0), DSP_MATH::q17_14_add(m_K_q, DSP_MATH::float_to_q17_14(1.0)));
        m_b1 = DSP_MATH::q17_14_divide(DSP_MATH::float_to_q17_14(-1.0), DSP_MATH::q17_14_add(m_K_q, DSP_MATH::float_to_q17_14(1.0)));
        m_a1 = DSP_MATH::q17_14_divide(DSP_MATH::q17_14_subtract(m_K_q, DSP_MATH::float_to_q17_14(1.0)), DSP_MATH::q17_14_add(m_K_q, DSP_MATH::float_to_q17_14(1.0)));
        m_b2 = 0;
        m_a2 = 0;
        break;
    case ALL_PASS_1:
        m_b0 = DSP_MATH::q17_14_divide(DSP_MATH::q17_14_subtract(m_K_q, DSP_MATH::float_to_q17_14(1.0)), DSP_MATH::q17_14_add(m_K_q, DSP_MATH::float_to_q17_14(1.0)));
        m_b1 = DSP_MATH::float_to_q17_14(1.0);
        m_a1 = DSP_MATH::q17_14_divide(DSP_MATH::q17_14_subtract(m_K_q, DSP_MATH::float_to_q17_14(1.0)), DSP_MATH::q17_14_add(m_K_q, DSP_MATH::float_to_q17_14(1.0)));
        m_b2 = 0;
        m_a2 = 0;
        break;
    case LOW_PASS_2:
    {
        int K2_Q_q = DSP_MATH::q17_14_multiply(K_square_q, Q_q);
        int det_q = DSP_MATH::q17_14_add(DSP_MATH::q17_14_add(K2_Q_q, m_K_q), Q_q);
        
        m_b0 = DSP_MATH::q17_14_divide(K2_Q_q, det_q);
        
        int nom_b1 = DSP_MATH::q17_14_multiply(DSP_MATH::float_to_q17_14(2.0), K2_Q_q);
        m_b1 = DSP_MATH::q17_14_divide(nom_b1, det_q);

        m_b2 = m_b0;
        
        int nom_a1 = DSP_MATH::q17_14_multiply(DSP_MATH::float_to_q17_14(2.0), DSP_MATH::q17_14_multiply(Q_q, DSP_MATH::q17_14_subtract(K_square_q, DSP_MATH::float_to_q17_14(1.0))));
        m_a1 = DSP_MATH::q17_14_divide(nom_a1, det_q);
        
        int nom_a2 = DSP_MATH::q17_14_add(DSP_MATH::q17_14_subtract(K2_Q_q, m_K_q), Q_q);
        m_a2 = DSP_MATH::q17_14_divide(nom_a2, det_q);
    }
    break;

    case HIGH_PASS_2:
    {
        int K2_Q_q = DSP_MATH::q17_14_multiply(K_square_q, Q_q);
        int det_q = DSP_MATH::q17_14_add(DSP_MATH::q17_14_add(K2_Q_q, m_K_q), Q_q);
        
        m_b0 = DSP_MATH::q17_14_divide(Q_q, det_q);
        m_b1 = DSP_MATH::q17_14_divide(DSP_MATH::q17_14_multiply(DSP_MATH::float_to_q17_14(-2.0), Q_q), det_q);
        m_b2 = m_b0; 

        int nom_a1 = DSP_MATH::q17_14_multiply(DSP_MATH::float_to_q17_14(2.0), DSP_MATH::q17_14_multiply(Q_q, DSP_MATH::q17_14_subtract(K_square_q, DSP_MATH::float_to_q17_14(1.0))));
        m_a1 = DSP_MATH::q17_14_divide(nom_a1, det_q);
        
        int nom_a2 = DSP_MATH::q17_14_add(DSP_MATH::q17_14_subtract(K2_Q_q, m_K_q), Q_q);
        m_a2 = DSP_MATH::q17_14_divide(nom_a2, det_q);
    }
    break;
    case BAND_PASS_2:
    {
        int det_q = DSP_MATH::q17_14_add(DSP_MATH::q17_14_add(DSP_MATH::q17_14_multiply(K_square_q, Q_q), m_K_q), Q_q);
        
        m_b0 = DSP_MATH::q17_14_divide(m_K_q, det_q);
        m_b1 = DSP_MATH::float_to_q17_14(0.0);
        m_b2 = DSP_MATH::q17_14_multiply(m_b0, DSP_MATH::float_to_q17_14(-1.0));

        int nom_a1 = DSP_MATH::q17_14_multiply(DSP_MATH::float_to_q17_14(2.0), DSP_MATH::q17_14_multiply(Q_q, DSP_MATH::q17_14_subtract(K_square_q, DSP_MATH::float_to_q17_14(1.0))));
        m_a1 = DSP_MATH::q17_14_divide(nom_a1, det_q);

        int nom_a2 = DSP_MATH::q17_14_add(DSP_MATH::q17_14_subtract(DSP_MATH::q17_14_multiply(K_square_q, Q_q), m_K_q), Q_q);
        m_a2 = DSP_MATH::q17_14_divide(nom_a2, det_q);
    }
    break;

    case BAND_REJECT_2:
    {
        int det_q = DSP_MATH::q17_14_add(DSP_MATH::q17_14_add(DSP_MATH::q17_14_multiply(K_square_q, Q_q), m_K_q), Q_q);
        
        int nom_b0 = DSP_MATH::q17_14_multiply(Q_q, DSP_MATH::q17_14_add(DSP_MATH::float_to_q17_14(1.0), K_square_q));
        m_b0 = DSP_MATH::q17_14_divide(nom_b0, det_q);

        m_b1 = DSP_MATH::q17_14_multiply(DSP_MATH::q17_14_multiply(DSP_MATH::float_to_q17_14(2.0), Q_q), DSP_MATH::q17_14_subtract(K_square_q, DSP_MATH::float_to_q17_14(1.0)));
        m_b1 = DSP_MATH::q17_14_divide(m_b1, det_q);

        m_b2 = m_b0;

        int nom_a1 = DSP_MATH::q17_14_multiply(DSP_MATH::q17_14_multiply(DSP_MATH::float_to_q17_14(2.0), Q_q), DSP_MATH::q17_14_subtract(K_square_q, DSP_MATH::float_to_q17_14(1.0)));
        m_a1 = DSP_MATH::q17_14_divide(nom_a1, det_q);

        int nom_a2 = DSP_MATH::q17_14_add(DSP_MATH::q17_14_subtract(DSP_MATH::q17_14_multiply(K_square_q, Q_q), m_K_q), Q_q);
        m_a2 = DSP_MATH::q17_14_divide(nom_a2, det_q);
    }
    break;
        
    case ALL_PASS_2:
    {
        int det_q = DSP_MATH::q17_14_add(DSP_MATH::q17_14_add(DSP_MATH::q17_14_multiply(K_square_q, Q_q), m_K_q), Q_q);

        int nom_b0 = DSP_MATH::q17_14_add(DSP_MATH::q17_14_subtract(DSP_MATH::q17_14_multiply(K_square_q, Q_q), m_K_q), Q_q);
        m_b0 = DSP_MATH::q17_14_divide(nom_b0, det_q);

        int nom_b1 = DSP_MATH::q17_14_multiply(DSP_MATH::q17_14_multiply(DSP_MATH::float_to_q17_14(2.0), Q_q), DSP_MATH::q17_14_subtract(K_square_q, DSP_MATH::float_to_q17_14(1.0)));
        m_b1 = DSP_MATH::q17_14_divide(nom_b1, det_q);

        m_b2 = DSP_MATH::float_to_q17_14(1.0);

        int nom_a1 = DSP_MATH::q17_14_multiply(DSP_MATH::q17_14_multiply(DSP_MATH::float_to_q17_14(2.0), Q_q), DSP_MATH::q17_14_subtract(K_square_q, DSP_MATH::float_to_q17_14(1.0)));
        m_a1 = DSP_MATH::q17_14_divide(nom_a1, det_q);

        int nom_a2 = DSP_MATH::q17_14_add(DSP_MATH::q17_14_subtract(DSP_MATH::q17_14_multiply(K_square_q, Q_q), m_K_q), Q_q);
        m_a2 = DSP_MATH::q17_14_divide(nom_a2, det_q);
    }
    break;

    default:
        m_b0 = m_b1 = m_b2 = m_a1 = m_a2 = 0;
        break;
    }
}

int Filter::CanonicalFilter::process(int input)
{
    int xh0 = DSP_MATH::q17_14_subtract(input, DSP_MATH::q17_14_add(DSP_MATH::q17_14_multiply(m_a1, m_xh1), DSP_MATH::q17_14_multiply(m_a2, m_xh2)));
    
    int term1 = DSP_MATH::q17_14_multiply(m_b0, xh0);
    int term2 = DSP_MATH::q17_14_multiply(m_b1, m_xh1);
    int term3 = DSP_MATH::q17_14_multiply(m_b2, m_xh2);
    
    int y = DSP_MATH::q17_14_add(DSP_MATH::q17_14_add(term1, term2), term3);

    m_xh2 = m_xh1;
    m_xh1 = xh0;

    return y;
}

void Filter::CanonicalFilter::process(std::vector<double> &inputSignal, std::vector<int> &outputSignal)
{
    outputSignal.clear(); 
    for(double sample : inputSignal) {
        int q_sample = DSP_MATH::float_to_q17_14(sample); 
        int output = this->process(q_sample); 
        outputSignal.push_back(output); 
    }
}
