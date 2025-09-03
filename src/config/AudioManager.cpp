#include "../includes/config/AudioManager.hpp"

bool AudioManager::loadSoundToBuffer(const std::string &filename, SourceID id, int &channels) {
    hu_alert("==> LOADING SOUND TO BUFFER...");

    SF_INFO sfinfo{};
    sfinfo.format = 0;
    SNDFILE *sndfile = sf_open(filename.c_str(), SFM_READ, &sfinfo);
    if(!sndfile) {
        std::cerr << "Failed to open WAV file: " << filename
          << " | Error: " << sf_strerror(nullptr) << "\n";
        return false;
    }

    int fileSampleRate = sfinfo.samplerate;
    channels = sfinfo.channels; // cập nhật số channel
    sf_count_t numFrames = sfinfo.frames;

    // Đọc buffer gốc từ file
    std::vector<double> buffer(numFrames * channels);
    sf_count_t readcount = sf_readf_double(sndfile, buffer.data(), numFrames);
    if(readcount != numFrames) {
        std::cerr << "Warning: read " << readcount << " frames, expected " << numFrames << "\n";
    }
    sf_close(sndfile);

    std::vector<double> resampledBuffer;

    // Nếu sample rate khác SAMP_FREQ thì resample
    if(fileSampleRate != SAMP_FREQ) {
        size_t newFrames = static_cast<size_t>(numFrames * SAMP_FREQ / fileSampleRate);
        resampledBuffer.resize(newFrames * channels);

        for(int ch = 0; ch < channels; ch++) {
            for(size_t i = 0; i < newFrames; i++) {
                double pos = i * static_cast<double>(numFrames) / newFrames;
                size_t idx = static_cast<size_t>(pos);
                double frac = pos - idx;
                double sample1 = buffer[idx * channels + ch];
                double sample2 = (idx + 1 < numFrames) ? buffer[(idx + 1) * channels + ch] : sample1;
                resampledBuffer[i * channels + ch] = sample1 * (1.0 - frac) + sample2 * frac;
            }
        }
    } else {
        resampledBuffer = std::move(buffer);
    }

    // Chọn buffer thành viên theo SourceID
    std::vector<double> *targetBuffer = nullptr;
    switch(id) {
        case SourceID::AUDIO_USAGE_MEDIA:                     targetBuffer = &m_mediaBuffer; break;
        case SourceID::AUDIO_USAGE_VOICE_COMMUNICATION:      targetBuffer = &m_voiceBuffer; break;
        case SourceID::AUDIO_USAGE_ASSISTANCE_NAVIGATION_GUIDANCE: targetBuffer = &m_assistanceBuffer; break;
        case SourceID::AUDIO_USAGE_NOTIFICATION:            targetBuffer = &m_notifBuffer; break;
        case SourceID::AUDIO_USAGE_ALARM:                   targetBuffer = &m_alarmBuffer; break;
        default:
            std::cerr << "Unknown SourceID!\n";
            return false;
    }

    *targetBuffer = std::move(resampledBuffer);

    hu_alert("==> LOADED SOUND TO BUFFER (resampled to " + std::to_string(SAMP_FREQ) + " Hz)");
    return true;
}


std::unique_ptr<AudioPipeline> AudioManager::createPipeline(SourceID id)
{
    auto pipeline = std::make_unique<AudioPipeline>();

    switch (id) {
        case SourceID::AUDIO_USAGE_MEDIA: {
            auto pToneControl = std::make_unique<Modules::ToneControl>(SAMP_FREQ);
            pToneControl->configuration(&mediaToneDefault.bass, 
                                        &mediaToneDefault.mid,
                                        &mediaToneDefault.treble);
            auto pLoudnessCompensation = std::make_unique<Modules::LoudnessCompensation>(SAMP_FREQ);
            auto pLimiter = std::make_unique<Modules::Limiter>(LIMITER_TH,
                                                               LIMITER_AT,
                                                               LIMITER_TH,
                                                               LIMITER_DELAY_SAMPLES);
            
            pipeline->addBlock(std::move(pLoudnessCompensation));
            pipeline->addBlock(std::move(pToneControl));
            pipeline->addBlock(std::move(pLimiter));
            break;
        }

        case SourceID::AUDIO_USAGE_VOICE_COMMUNICATION: {
            auto pLimiter = std::make_unique<Modules::Limiter>(LIMITER_TH,
                                                               LIMITER_AT,
                                                               LIMITER_TH,
                                                               LIMITER_DELAY_SAMPLES);
            pipeline->addBlock(std::move(pLimiter));
            break;
        }

        case SourceID::AUDIO_USAGE_ASSISTANCE_NAVIGATION_GUIDANCE: {
            auto pToneControl = std::make_unique<Modules::ToneControl>(SAMP_FREQ); // Mid-boost 1–2kHz
            auto pLimiter = std::make_unique<Modules::Limiter>(LIMITER_TH,
                                                               LIMITER_AT,
                                                               LIMITER_TH,
                                                               LIMITER_DELAY_SAMPLES);
            
            pipeline->addBlock(std::move(pToneControl));
            pipeline->addBlock(std::move(pLimiter));
            break;
        }

        case SourceID::AUDIO_USAGE_ALARM: {
            auto pLimiter = std::make_unique<Modules::Limiter>(LIMITER_TH,
                                                               LIMITER_AT,
                                                               LIMITER_TH,
                                                               LIMITER_DELAY_SAMPLES);
            pipeline->addBlock(std::move(pLimiter));
            break;
        }

        case SourceID::AUDIO_USAGE_NOTIFICATION: {
            auto pLimiter = std::make_unique<Modules::Limiter>(LIMITER_TH,
                                                               LIMITER_AT,
                                                               LIMITER_TH,
                                                               LIMITER_DELAY_SAMPLES);
            pipeline->addBlock(std::move(pLimiter));
            break;
        }
    }

    return pipeline;
}

void AudioManager::init() {
    int channels;

    /* We have 5 sources in the system */
    loadSoundToBuffer("sounds/media.wav", SourceID::AUDIO_USAGE_MEDIA, channels);
    loadSoundToBuffer("sounds/voicephone.wav", SourceID::AUDIO_USAGE_VOICE_COMMUNICATION, channels);
    loadSoundToBuffer("sounds/assistance.wav", SourceID::AUDIO_USAGE_ASSISTANCE_NAVIGATION_GUIDANCE, channels);
    loadSoundToBuffer("sounds/notif.wav", SourceID::AUDIO_USAGE_NOTIFICATION, channels);
    loadSoundToBuffer("sounds/alarm.wav", SourceID::AUDIO_USAGE_ALARM, channels);

    /* Create pipeline for each source */
    m_mediaPipeline      = createPipeline(SourceID::AUDIO_USAGE_MEDIA);
    m_voicePipeline      = createPipeline(SourceID::AUDIO_USAGE_VOICE_COMMUNICATION);
    m_assistancePipeline = createPipeline(SourceID::AUDIO_USAGE_ASSISTANCE_NAVIGATION_GUIDANCE);
    m_notifPipeline      = createPipeline(SourceID::AUDIO_USAGE_NOTIFICATION);
    m_alarmPipeline      = createPipeline(SourceID::AUDIO_USAGE_ALARM);
}
