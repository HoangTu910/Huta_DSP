#ifndef _AUDIO_MANAGER_HPP_
#define _AUDIO_MANAGER_HPP_

#define NUM_SOURCE 5

#include "../pipeline/AudioPipeline.hpp"
#include "../modules/Limiter.hpp"
#include "../modules/LoudnessCompensation.hpp"
#include "../modules/ToneControl.hpp"
#include "SourceSelector.hpp"
#include <sndfile.h>

class AudioManager {
public:
    bool loadSoundToBuffer(const std::string &filename, SourceID id, int &channels);
    std::unique_ptr<AudioPipeline> createPipeline(SourceID id);
    
    void init();
private:
    std::vector<std::unique_ptr<AudioPipeline>> pipelines{NUM_SOURCE};
    std::vector<double> m_mediaBuffer, m_voiceBuffer, m_assistanceBuffer, m_notifBuffer, m_alarmBuffer;
    std::unique_ptr<AudioPipeline> m_mediaPipeline, 
                                   m_voicePipeline, 
                                   m_assistancePipeline, 
                                   m_notifPipeline, 
                                   m_alarmPipeline   ;     
    int m_mediaChannel, 
        m_voiceChannel, 
        m_assistanceChannel, 
        m_notifChannel, 
        m_alarmChannel;
};

#endif