#include "../core/BiquadFilter.hpp"
#include <vector>

namespace Modules {

class LoudnessCompensation {
public:
    LoudnessCompensation(int fs) {
        m_bassShelf = std::make_unique<Filter::BiquadFilter>(fs);
        m_trebleShelf = std::make_unique<Filter::BiquadFilter>(fs);
    }

    struct LoudnessPoint {
        int volumeStep;
        double bassGainDb;
        double trebleGainDb;
    };

    void applyLoudnessCompensation(std::vector<float>& audioBuffer);
private:
    std::unique_ptr<Filter::BiquadFilter> m_bassShelf;
    std::unique_ptr<Filter::BiquadFilter> m_trebleShelf;
    double m_bassFreq;
    double m_trebleFreq;
    std::vector<LoudnessPoint> m_loudnessCurve;
};

}