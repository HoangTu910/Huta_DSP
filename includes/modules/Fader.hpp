#ifndef _FADER_H_
#define _FADER_H_

#include "../interface/IDSPBlock.hpp"
#include "../dsp_math/QNumber.hpp"
#include "../dsp_math/common.hpp"
#include "../common/filterType.hpp"

namespace Modules
{
class Fader : public IDSPBlock{
public:
    Fader(double slopeLinear, double slopeExp, FaderType type, double target) 
        : m_slopeLinear(slopeLinear), m_slopeExp(slopeExp), m_mode(type), m_targetGain(target)
    {}

    void setTarget(double target);
    void mute();
    void unmute(double volume);
    double process(double sample);
    void process(std::vector<double>& buffer) override;

private:
    double m_currentGain;
    double m_targetGain;
    double m_slopeLinear;
    double m_slopeExp;
    FaderType m_mode;
    void vCalculateGain();
};
} 


#endif