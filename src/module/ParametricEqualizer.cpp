#include "../../includes/module/ParametricEqualizer.hpp"

void Equalizer::ParametricEqualizer::setLowShelf(int fc, double factor)
{
    m_lowShelf->setParameters(Type::LowShelf, fc, factor, 0);
}

void Equalizer::ParametricEqualizer::setHighShelf(int fc, double factor)
{
    m_lowShelf->setParameters(Type::HighShelf, fc, factor, 0);
}

void Equalizer::ParametricEqualizer::setFirstMidPeak(int fc, double G, double Q)
{
    m_firstMidPeak->setParameters(Type::Peaking, fc, Q, G);
}

void Equalizer::ParametricEqualizer::setSecondMidPeak(int fc, double G, double Q)
{
    m_secondMidPeak->setParameters(Type::Peaking, fc, Q, G);
}

int Equalizer::ParametricEqualizer::process(int sample) {
    int outputLowShelf = m_lowShelf->process(sample);
    int outputFirstMidPeak = m_firstMidPeak->process(outputLowShelf);
    int outputSecondMidPeak = m_secondMidPeak->process(outputFirstMidPeak);
    int outputSample = m_highShelf->process(outputSecondMidPeak);

    return outputSample;
}

std::vector<int> Equalizer::ParametricEqualizer::process(std::vector<double> &inputSignal)
{
    std::vector<int> outputSignal;
    for(double sample : inputSignal) {
        int q_sample = DSP_MATH::int_to_q16_15(sample);
        outputSignal.push_back(process(q_sample));
    }

    return outputSignal;
}
