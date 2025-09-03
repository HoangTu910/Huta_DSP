#ifndef SOURCE_SELECTOR_HPP
#define SOURCE_SELECTOR_HPP

#include <vector>

const int TOTAL_SOURCE_INPUT = 5;

enum class SourceID {
    AUDIO_USAGE_MEDIA = 0,
    AUDIO_USAGE_VOICE_COMMUNICATION,
    AUDIO_USAGE_ASSISTANCE_NAVIGATION_GUIDANCE,
    AUDIO_USAGE_ALARM,
    AUDIO_USAGE_NOTIFICATION
};

struct AudioSource {
    SourceID id;
    bool isActive;
    std::vector<double> buffer;
};

class SourceSelector {
public:
    void setActive(SourceID id, bool isActive); 
    float getDuckingFactor() const;
    std::vector<AudioSource> m_sources{TOTAL_SOURCE_INPUT};
private:
    std::vector<double> m_mediaBuffer, m_voiceBuffer, m_assistanceBuffer, m_notifBuffer, m_alarmBuffer;
};

#endif // SOURCE_SELECTOR_HPP