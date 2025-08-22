#include "../../includes/interface/IFilter.hpp"

void IFilter::normalize(std::vector<double> &coeff)
{
    return;
}

void IFilter::convertQtoF(const std::vector<int> &inputSignal, std::vector<double> &outputSignal)
{
    outputSignal.clear();
    outputSignal.reserve(inputSignal.size());
    for(size_t i = 0; i < inputSignal.size(); i++) {
        outputSignal.push_back(DSP_MATH::q16_15_to_float(inputSignal[i]));
    }
}
