#ifndef _SVF_H_
#define _SVF_H_

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#include "../interface/IFilter.hpp"
#include "../common/debug.hpp"
#include "../common/filterType.hpp"
#include "../dsp_math/QNumber.hpp"
#include <iostream>
#include <vector>
#include <cmath>
#include <numeric>

typedef struct {
    int yl;
    int yb;
    int yh;
} output;

namespace Filter {
class StateVariableFilter : public IFilter {
public:
    StateVariableFilter(int fs) {
        m_fs = fs;
        m_yl0 = 0;
        m_yb0 = 0;
        m_yh0 = 0;
        m_yl1 = 0;
        m_yb1 = 0;
        m_yh1 = 0;
    };
    void tuning(int fc, double Q);
    int process(int input) override;
    void process(std::vector<double> &inputSignal, std::vector<int> &h_outputSignal) override;
    output processAll(int input);
    void process(std::vector<double> &inputSignal, std::vector<int> &h_outputSignal, 
                 std::vector<int> &b_outputSignal, std::vector<int> &l_outputSignal);
    void normalize(std::vector<double> &coeff) override;

private:
    int m_fs, m_fc;
    int m_yl0, m_yl1;
    int m_yb0, m_yb1;
    int m_yh0, m_yh1;
    int m_F1, m_Q1;
};
};

#endif
