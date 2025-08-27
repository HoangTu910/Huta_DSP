#include "../../includes/module/ParametricEqualizer.hpp"
#define HUDEBUG

void Equalizer::ParametricEqualizer::setLowShelf(int fc, double factor)
{
    m_lowShelf->setParameters(Type::LowShelf, fc, 0, factor);
}

void Equalizer::ParametricEqualizer::setHighShelf(int fc, double factor)
{
    m_highShelf->setParameters(Type::HighShelf, fc, 0, factor);
}

void Equalizer::ParametricEqualizer::setFirstMidPeak(int fc, double G, double Q)
{
    m_firstMidPeak->setParameters(Type::Peaking, fc, Q, G);
}

void Equalizer::ParametricEqualizer::setSecondMidPeak(int fc, double G, double Q)
{
    m_secondMidPeak->setParameters(Type::Peaking, fc, Q, G);
}

void Equalizer::ParametricEqualizer::setShelfSlope(int typeShelf, float shelfSlope)
{
    if(typeShelf == HIGH_SHELF) {
        m_highShelf->setShelfSlope(shelfSlope);
    }
    else if(typeShelf == LOW_SHELF) {
        m_lowShelf->setShelfSlope(shelfSlope);
    }
    else {
        /* If don't set the shelf type -> set all by default*/
        m_highShelf->setShelfSlope(shelfSlope);
        m_lowShelf->setShelfSlope(shelfSlope);
    }
}

bool check = true;
int cnt = 0;

int Equalizer::ParametricEqualizer::process(int sample) {
    int outputLowShelf = m_lowShelf->process(sample);
    int outputFirstMidPeak = m_firstMidPeak->process(outputLowShelf);
    int outputSecondMidPeak = m_secondMidPeak->process(outputFirstMidPeak);
    int outputSample = m_highShelf->process(outputSecondMidPeak);
    cnt++;
    #ifdef HUDEBUG
    if(check && cnt == 10) {
        hu_debug(cnt);
        hu_debug(sample);
        hu_debug(outputLowShelf);
        hu_debug(outputFirstMidPeak);
        hu_debug(outputSecondMidPeak);
        hu_debug(outputSample);
        check = false;
    }
    #endif

    return outputSample;
}

std::vector<int> Equalizer::ParametricEqualizer::process(std::vector<double> &inputSignal)
{
    std::vector<int> outputSignal;
    for(double sample : inputSignal) {
        int q_sample = DSP_MATH::float_to_q16_15(sample);
        outputSignal.push_back(process(q_sample));
    }

    return outputSignal;
}
