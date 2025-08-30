#include <iostream>
#include <vector>
#include <string>
#include <cmath>     // For std::sin and std::cos
#include <stdexcept> // For std::runtime_error

#define M_PI 3.14159265358979323846

// A handy alias for our data structures to make the code readable
using AudioBuffer = std::vector<double>;
using MultiChannelBuffer = std::vector<AudioBuffer>;

// --- We will use simple "dummy" classes for the modules ---
// --- Their only job is to print a message so we can trace the flow ---

class ToneControl {
public:
    AudioBuffer process(const AudioBuffer& input) {
        std::cout << "    -> Applying Tone Control..." << std::endl;
        // In a real system, this would apply bass/treble EQ. Here we just pass it through.
        return input;
    }
};

class SpeakerEQ {
public:
    AudioBuffer process(const AudioBuffer& input) {
        std::cout << "    -> Applying Speaker Correction EQ..." << std::endl;
        return input;
    }
};

class Limiter {
public:
    AudioBuffer process(const AudioBuffer& input) {
        std::cout << "    -> Applying Limiter..." << std::endl;
        return input;
    }
};

// --- This is the REAL implementation of the Matrix Mixer ---

class MatrixMixer {
public:
    using GainMatrix = std::vector<std::vector<double>>;

    void configure(const GainMatrix& matrix) {
        m_gainMatrix = matrix;
        m_numOutputs = m_gainMatrix.size();
        if (m_numOutputs > 0) {
            m_numInputs = m_gainMatrix[0].size();
        } else {
            m_numInputs = 0;
        }
    }

    MultiChannelBuffer process(const MultiChannelBuffer& inputChannels) {
        if (inputChannels.size() != m_numInputs) {
            throw std::runtime_error("MatrixMixer: Input channel count mismatch.");
        }
        if (m_numOutputs == 0 || m_numInputs == 0) {
            return {};
        }

        const size_t numSamples = inputChannels[0].size();
        MultiChannelBuffer outputChannels(m_numOutputs, AudioBuffer(numSamples, 0.0));
        std::cout << "numSamples: " << numSamples << std::endl;
        std::cout << "m_numInputs: " << m_numInputs << std::endl;
        std::cout << "m_numOutputs: " << m_numOutputs << std::endl;

        for (size_t i = 0; i < numSamples; ++i) {
            for (size_t m = 0; m < m_numOutputs; ++m) {
                double sampleSum = 0.0;
                for (size_t n = 0; n < m_numInputs; ++n) {
                    sampleSum += inputChannels[n][i] * m_gainMatrix[m][n];
                }
                outputChannels[m][i] = sampleSum;
            }
        }
        return outputChannels;
    }

private:
    GainMatrix m_gainMatrix;
    size_t m_numInputs{0};
    size_t m_numOutputs{0};
};

// --- This is our main Audio Pipeline, tying everything together ---

class AudioPipeline {
public:
    AudioPipeline() {
        std::cout << "[Pipeline Setup] Configuring for 2.1 (Stereo In -> L/R/Sub Out)\n" << std::endl;

        // This simulates loading the 2x3 matrix from a config file
        MatrixMixer::GainMatrix upmixMatrix = {
            // Output 0 (Left Speaker)  : 100% from Input 0 (L), 0% from Input 1 (R)
            { 1.0, 0.0 },
            // Output 1 (Right Speaker) : 0% from Input 0 (L), 100% from Input 1 (R)
            { 0.0, 1.0 },
            // Output 2 (Subwoofer)     : 50% from Input 0 (L), 50% from Input 1 (R)
            { 0.5, 0.5 }
        };
        m_mixer.configure(upmixMatrix);

        // Create the per-channel post-processing modules
        m_speakerEQs.resize(3); // We have 3 output channels
        m_limiters.resize(3);   // We have 3 output channels
    }

    MultiChannelBuffer process(const MultiChannelBuffer& inputChannels) {
        std::cout << "[Pipeline Processing] Starting with " << inputChannels.size() << " input channels..." << std::endl;

        // --- STAGE 1 & 2: Pre-Processing ---
        MultiChannelBuffer processedPreMix = inputChannels; // Start with a copy
        for (auto& channel : processedPreMix) {
            channel = m_toneControl.process(channel);
            // ... LoudnessCompensation would go here ...
        }

        // --- STAGE 3: THE GREAT TRANSFORMATION ---
        std::cout << "\n[Pipeline Processing] Sending 2 channels to the Matrix Mixer..." << std::endl;
        MultiChannelBuffer mixedChannels = m_mixer.process(processedPreMix);
        std::cout << "[Pipeline Processing] Received " << mixedChannels.size() << " channels from the Matrix Mixer!\n" << std::endl;

        // --- STAGE 4: Post-Processing on M channels ---
        MultiChannelBuffer finalOutput = mixedChannels;
        for (size_t i = 0; i < finalOutput.size(); ++i) {
            std::cout << "  Processing Output Channel " << i << ":" << std::endl;
            finalOutput[i] = m_speakerEQs[i].process(finalOutput[i]);
            finalOutput[i] = m_limiters[i].process(finalOutput[i]);
        }

        std::cout << "\n[Pipeline Processing] Finished. Final output has " << finalOutput.size() << " channels." << std::endl;
        return finalOutput;
    }

private:
    ToneControl m_toneControl;
    MatrixMixer m_mixer;
    std::vector<SpeakerEQ> m_speakerEQs;
    std::vector<Limiter> m_limiters;
};

// --- Helper functions to simulate reading and writing files ---

// Simulates a "Decoder Service" reading a stereo WAV file.
MultiChannelBuffer createFakeStereoSignal() {
    const double sampleRate = 44100.0;
    const double duration = 1.0; // 1 second
    const double frequency = 440.0; // A4 note
    const size_t numSamples = static_cast<size_t>(sampleRate * duration);

    MultiChannelBuffer stereoSignal(2, AudioBuffer(numSamples));

    for (size_t i = 0; i < numSamples; ++i) {
        double time = i / sampleRate;
        // Left channel gets a sine wave
        stereoSignal[0][i] = 0.5 * std::sin(2.0 * M_PI * frequency * time);
        // Right channel gets a cosine wave (so they are different)
        stereoSignal[1][i] = 0.5 * std::cos(2.0 * M_PI * frequency * time);
    }
    std::cout << "[File System] Created a fake stereo signal with " << numSamples << " samples.\n" << std::endl;
    return stereoSignal;
}

// Simulates the final AFE writing the output channels to separate files.
void saveOutputChannels(const MultiChannelBuffer& outputChannels) {
    std::cout << "\n--- Final Output Analysis ---" << std::endl;
    for (size_t i = 0; i < outputChannels.size(); ++i) {
        printf("Output Channel %zu (e.g., Speaker %zu):\n", i, i + 1);
        printf("  - Sample 0: %.3f\n", outputChannels[i][0]);
        printf("  - Sample 1: %.3f\n", outputChannels[i][1]);
        printf("  - Sample 2: %.3f\n", outputChannels[i][2]);
    }
}

// --- Main Function ---

int main() {
    // 1. DECODER: Simulate reading a stereo .wav file into a 2D vector.
    MultiChannelBuffer inputSignal = createFakeStereoSignal();

    // 2. PIPELINE: Create the main processing object.
    AudioPipeline pipeline;

    // 3. PROCESS: Run the entire audio chain.
    MultiChannelBuffer outputSignal = pipeline.process(inputSignal);

    // 4. AFE: Simulate saving the final M channels.
    saveOutputChannels(outputSignal);

    return 0;
}