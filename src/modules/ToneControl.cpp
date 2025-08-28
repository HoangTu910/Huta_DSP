#include "../includes/modules/ToneControl.hpp"

void Module::ToneControl::setBassFilter(int fc, double factor) {
    m_bassFilter->setParameters(Type::LowShelf, fc, 0, factor);
}

void Module::ToneControl::setTrebleFilter(int fc, double factor)
{
    m_trebleFilter->setParameters(Type::HighShelf, fc, 0, factor);
}

void Module::ToneControl::setMidFilter(int fc, double G, double Q)
{
    m_midFilter->setParameters(Type::Peaking, fc, Q, G);
}

void Module::ToneControl::setShelfSlope(int typeShelf, float shelfSlope)
{
    if(typeShelf == HIGH_SHELF) {
        m_trebleFilter->setShelfSlope(shelfSlope);
    }
    else if(typeShelf == LOW_SHELF) {
        m_bassFilter->setShelfSlope(shelfSlope);
    }
    else {
        /* If don't set the shelf type -> set all by default*/
        m_trebleFilter->setShelfSlope(shelfSlope);
        m_bassFilter->setShelfSlope(shelfSlope);
    }
}

void Module::ToneControl::configuration(const ToneControlParams *bass, const ToneControlParams *mid, const ToneControlParams *treble)
{
    /*
     * Gain will be set to 0 (flat) when init configuration 
     */
    m_bassFilter->setParameters(Type::LowShelf, bass->freq, bass->Q, N0_F);
    m_midFilter->setParameters(Type::Peaking, mid->freq, mid->Q, N0_F);
    m_trebleFilter->setParameters(Type::HighShelf, treble->freq, treble->Q, N0_F);
}

int Module::ToneControl::process(int sample) {
    int outputLowShelf = m_bassFilter->process(sample);
    int outputMidPeak = m_midFilter->process(outputLowShelf);
    int outputSample = m_trebleFilter->process(outputMidPeak);
    
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

std::vector<int> Module::ToneControl::process(std::vector<double> &inputSignal) {
    std::vector<int> outputSignal;
    for(double sample : inputSignal) {
        int q_sample = DSP_MATH::float_to_q16_15(sample);
        outputSignal.push_back(process(q_sample));
    }

    return outputSignal;
}

