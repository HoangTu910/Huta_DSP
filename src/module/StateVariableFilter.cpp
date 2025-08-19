#include "../../includes/module/StateVariableFilter.hpp"

void Filter::StateVariableFilter::tuning(int fc, double Q)
{
    int q_pi = DSP_MATH::float_to_q17_14(M_PI);
    int q_fc = DSP_MATH::int_to_q17_14(fc);
    m_fc = q_fc;
    int pi_fc = DSP_MATH::q17_14_multiply(q_pi, q_fc);
    int div = DSP_MATH::q17_14_divide(pi_fc, m_fs);
    int q_sin = DSP_MATH::q17_14_sin(div);
    m_F1 = DSP_MATH::q17_14_multiply(q_sin, DSP_MATH::int_to_q17_14(2));
    m_Q1 = DSP_MATH::q17_14_divide(DSP_MATH::int_to_q17_14(1), DSP_MATH::float_to_q17_14(Q));
    hu_debug(m_fc);
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
    int temp = DSP_MATH::q17_14_subtract(input, m_yl1);
    int mul = DSP_MATH::q17_14_multiply(m_Q1, m_yb1);
    m_yh0 = DSP_MATH::q17_14_subtract(temp, mul);

    // yb(n) = F1 * yh(n) + yb(n-1)
    mul = DSP_MATH::q17_14_multiply(m_F1, m_yh0);
    m_yb0 = DSP_MATH::q17_14_add(mul, m_yb1);

    // yl(n) = F1 * yb(n) + yl(n-1)
    mul = DSP_MATH::q17_14_multiply(m_F1, m_yb0);
    m_yl0 = DSP_MATH::q17_14_add(mul, m_yl1);

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

