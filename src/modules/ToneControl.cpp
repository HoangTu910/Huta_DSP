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
    m_bassFilter->setParameters(Type::LowShelf, bass->freq, bass->Q, bass->gain);
    m_midFilter->setParameters(Type::Peaking, mid->freq, mid->Q, mid->gain);
    m_trebleFilter->setParameters(Type::HighShelf, treble->freq, treble->Q, treble->gain);
}
