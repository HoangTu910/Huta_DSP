#include "../includes/modules/ToneControl.hpp"

TonePreset mediaToneDefault = {
    {100, 0.0, 3.0},     // Bass 100Hz, +3dB
    {1000, 1.0, 2.0},    // Mid 1kHz, Q=1, +2dB
    {8000, 0.0, 4.0}    // Treble 10kHz, +4dB
};

void Modules::ToneControl::setBassFilter(int fc, double factor) {
    m_bassFilter->setParameters(Type::LowShelf, fc, 0, factor);
}

void Modules::ToneControl::setTrebleFilter(int fc, double factor)
{
    m_trebleFilter->setParameters(Type::HighShelf, fc, 0, factor);
}

void Modules::ToneControl::setMidFilter(int fc, double G, double Q)
{
    m_midFilter->setParameters(Type::Peaking, fc, Q, G);
}

void Modules::ToneControl::setShelfSlope(int typeShelf, float shelfSlope)
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

void Modules::ToneControl::configuration(const ToneControlParams *bass, const ToneControlParams *mid, const ToneControlParams *treble)
{
    /*
     * Gain will be set to 0 (flat) when init configuration 
     */
    m_bassFilter->setParameters(Type::LowShelf, bass->freq, bass->Q, N0_F);
    m_midFilter->setParameters(Type::Peaking, mid->freq, mid->Q, N0_F);
    m_trebleFilter->setParameters(Type::HighShelf, treble->freq, treble->Q, N0_F);
}

int Modules::ToneControl::process(int sample) {
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

void Modules::ToneControl::process(std::vector<double> &inputSignal) {
    for(double &sample : inputSignal) {
        int q_sample = DSP_MATH::float_to_q16_15(sample);
        sample = process(q_sample);
    }
}

