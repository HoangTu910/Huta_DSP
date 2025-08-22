#include "../../includes/module/StateVariableFilter.hpp"

void Filter::StateVariableFilter::tuning(int fc, double Q)
{
    m_yh0 = m_yh1 = 0;
    m_yb0 = m_yb1 = 0;
    m_yl0 = m_yl1 = 0;

    double fc_d = static_cast<double>(fc);
    double pi_d = M_PI;
    double Q_d = Q;

    double pi_fc_d = pi_d * fc_d;
    double div_d = pi_fc_d / static_cast<double>(m_fs);
    double sin_d = sin(div_d);

    double F1_d = 2.0 * sin(div_d); 
    double Q1_d = 1.0 / Q_d;

    m_fc = fc;
    m_F1 = DSP_MATH::float_to_q16_15(F1_d);
    m_Q1 = DSP_MATH::float_to_q16_15(Q1_d);

    hu_debug(F1_d);
    hu_debug(Q1_d);
    hu_debug(m_F1);
    hu_debug(m_Q1);
}

int Filter::StateVariableFilter::process(int input)
{
    return 0; // unused
}

void Filter::StateVariableFilter::process(std::vector<double> &inputSignal, std::vector<int> &h_outputSignal)
{
    return; // unused
}

output Filter::StateVariableFilter::processAll(int input) {
    output y;

    // yh(n) = x(n) - yl(n-1) - Q1 * yb(n-1)
    int temp = DSP_MATH::q16_15_subtract(input, m_yl1);
    int mul = DSP_MATH::q16_15_multiply(m_Q1, m_yb1);
    m_yh0 = DSP_MATH::q16_15_subtract(temp, mul);

    // yb(n) = F1 * yh(n) + yb(n-1)
    mul = DSP_MATH::q16_15_multiply(m_F1, m_yh0);
    m_yb0 = DSP_MATH::q16_15_add(mul, m_yb1);

    // yl(n) = F1 * yb(n) + yl(n-1)
    mul = DSP_MATH::q16_15_multiply(m_F1, m_yb0);
    m_yl0 = DSP_MATH::q16_15_add(mul, m_yl1);

    y.yl = m_yl0;
    y.yb = m_yb0;
    y.yh = m_yh0;

    m_yl1 = m_yl0;
    m_yb1 = m_yb0;
    m_yh1 = m_yh0;

    return y;
}

void Filter::StateVariableFilter::process(std::vector<double> &inputSignal, std::vector<int> &h_outputSignal, std::vector<int> &b_outputSignal, std::vector<int> &l_outputSignal)
{
    auto processedSignal = DSP_MATH::preprocessQNumber(inputSignal);
    h_outputSignal.clear();
    b_outputSignal.clear();
    l_outputSignal.clear();

    for (const auto &sample : processedSignal) {
        output y = processAll(sample);
        h_outputSignal.push_back(y.yh);
        b_outputSignal.push_back(y.yb);
        l_outputSignal.push_back(y.yl);
    }
}

void Filter::StateVariableFilter::normalize(std::vector<double> &coeff) { return; } //unused

