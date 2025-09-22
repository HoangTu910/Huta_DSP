#ifndef _FADER_H_
#define _FADER_H_

#include "../interface/IDSPBlock.hpp"
#include "../dsp_math/QNumber.hpp"
#include "../dsp_math/common.hpp"

namespace Modules
{
class Fader : public IDSPBlock{
public:
    Fader(double slopeLinear, double slopeExp) 
        : m_slopeLinear(slopeLinear), m_slopeExp(slopeExp)
    {}

    void setTarget(double target);
    void mute();
    void unmute();
    void process(std::vector<double>& buffer) override;

private:
    double m_currentGain;
    double m_targetGain;
    double m_slopeLinear;
    double m_slopeExp;
    bool m_mode;
};
} 


#endif