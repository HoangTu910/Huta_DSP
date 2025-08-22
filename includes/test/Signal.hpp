#ifndef _SIGNAL_H_
#define _SIGNAL_H_ 

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#include "../common/filterType.hpp"
#include <vector>
#include <numeric>
#include <cmath>
#include <algorithm>
#include <fstream>
#include <sndfile.h>
#include <iostream>

const int MAX_TEST_SIGNALS = 10;

class Signal {
public:
    Signal(int samp_freq, int duration, double amplitude) 
        : m_sampFreq(samp_freq),
          m_duration(duration),
          m_amplitude(amplitude),
          t_outputSignal(MAX_TEST_SIGNALS) 
    {
        for (int i = 0; i < samp_freq * duration; ++i) {
            double time = static_cast<double>(i) / samp_freq;
            double mixed_signal = amplitude * sin(2 * M_PI * 100 * time);
            mixed_signal += amplitude * 0.3 * sin(2 * M_PI * 200 * time);
            mixed_signal += amplitude * 0.2 * sin(2 * M_PI * 400 * time);
            mixed_signal += amplitude * 0.1 * sin(2 * M_PI * 800 * time);
            mixed_signal += amplitude * 0.5 * sin(2 * M_PI * 900 * time);
            t_inputSignal.push_back(mixed_signal);
        }
        this->createSoundSignal(t_inputSignal, SoundTestFiles::Input);
    }

    void generateTestSignal(std::vector<double> &signal, int samp_freq, int duration, int amplitude);

    std::vector<double> t_inputSignal;
    std::vector<std::vector<int>> t_outputSignal;  

    template<typename TT>
    void writeSignal(const std::vector<TT> &signal, const std::string &filename) {
        std::ofstream outFile(filename);
        if (outFile.is_open()) {
            for (size_t i = 0; i < signal.size(); ++i) {
                outFile << i / static_cast<double>(m_sampFreq) << " " << signal[i] << "\n";
            }
            outFile.close();
            std::cout << "Write done " << filename << std::endl;
        } else {
            std::cout << "Error writing to " << filename << std::endl;
        }
    }

    template<typename TT>
    void scale(const std::vector<TT> &inputSignal, std::vector<double> &outputSignal)
    {
        outputSignal.resize(inputSignal.size());
        double maxVal = 0;
        for (size_t i = 0; i < inputSignal.size(); i++)
            if (std::abs(inputSignal[i]) > maxVal)
                maxVal = std::abs(inputSignal[i]);

        for (size_t i = 0; i < inputSignal.size(); i++)
            outputSignal[i] = static_cast<double>(inputSignal[i]) / maxVal;
    }

    template<typename TT>
    void createSoundSignal(std::vector<TT> &signal, const std::string &filename)
    {
        SF_INFO sfInfo;
        sfInfo.samplerate = m_sampFreq;
        sfInfo.channels = 1; //mono
        sfInfo.format = SF_FORMAT_WAV | SF_FORMAT_PCM_16;
        SNDFILE* outFile = sf_open(filename.c_str(), SFM_WRITE, &sfInfo);
        if (!outFile) {
            std::cerr << "Error create .wav\n";
            return;
        }
        std::vector<double> out;
        this->scale(signal, out);
        sf_writef_double(outFile, out.data(), out.size() / sfInfo.channels);
        sf_close(outFile);
        std::cout << "Sound file done .wav\n";
    }

private:
    int m_sampFreq;
    int m_duration;
    double m_amplitude;

    void writeMixedSignal();
    void writeFilteredSignal();
};

#endif
