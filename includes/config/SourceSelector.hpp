#ifndef SOURCE_SELECTOR_HPP
#define SOURCE_SELECTOR_HPP

#include <vector>

enum class SourceID {
    AUDIO_USAGE_MEDIA,
    AUDIO_USAGE_VOICE_COMMUNICATION,
    AUDIO_USAGE_ASSISTANCE_NAVIGATION_GUIDANCE,
    AUDIO_USAGE_ALARM,
    AUDIO_USAGE_NOTIFICATION
};

struct Source {
    SourceID id;
    bool isActive;
    std::vector<double> buffer;
};

class SourceSelector {
public:
    void setActive(SourceID id, bool isActive); 
    float getDuckingFactor() const;
};

#endif // SOURCE_SELECTOR_HPP