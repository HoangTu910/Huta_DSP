#include "../../includes/module/ParametricEqualizer.hpp"

void Equalizer::ParametricEqualizer::setLowShelf(int type, int fc, double factor)
{
    m_lowShelf->setType(type, fc, factor);
}

void Equalizer::ParametricEqualizer::setHighShelf(int type, int fc, double factor)
{
    m_highShelf->setType(type, fc, factor);
}

void Equalizer::ParametricEqualizer::setFirstMidPeak(int type, int fc, double G, double Q)
{
    m_firstMidPeak->setType(type, fc, G, Q);
}

void Equalizer::ParametricEqualizer::setSecondMidPeak(int type, int fc, double G, double Q)
{
    m_secondMidPeak->setType(type, fc, G, Q);
}

int Equalizer::ParametricEqualizer::process(int sample) {
    int outputLowShefl = m_lowShelf->process(sample);
    int outputFirstMidPeak = m_firstMidPeak->process(outputLowShefl);
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
