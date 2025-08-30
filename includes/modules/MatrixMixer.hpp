#ifndef _MATRIX_MIXER_HPP
#define _MATRIX_MIXER_HPP

#include <vector>

/* 
 * For simulation purposes, we will assume the car system has 4 speaker 
 * Front Left, Front Right, Rear Left, Rear Right
 * 
 * For the input channel, we will have 
 *  AUDIO_USAGE_MEDIA (music) -> stereo
 *  AUDIO_USAGE_VOICE_COMMUNICATION (voice) -> mono
 *  AUDIO_USAGE_ASSISTANCE_NAVIGATION_GUIDANCE (GPS navigation) -> mono
 *  AUDIO_USAGE_ALARM (alarm) -> mono
 *  AUDIO_USAGE_NOTIFICATION (notification) -> mono
 */
#define NUMBER_OUTPUT_CHANNELS 4
#define NUMBER_INPUT_CHANNELS 6

class MatrixMixer {
public:
    MatrixMixer(int numSources, int numOutputs) 
    : m_numInputs(numSources), m_numOutputs(numOutputs), m_gainMatrix(numOutputs, std::vector<float>(numSources, 0.0f)) {}

    void setSourceBuffer(int sourceIndex, const std::vector<double>& buffer);
    void process();
    std::vector<double> getOutputBuffer(int outputIndex) const;

private:
    int m_numInputs;
    int m_numOutputs;
    std::vector<std::vector<float>> m_gainMatrix; // [output][input]
};

#endif // _MATRIX_MIXER_HPP