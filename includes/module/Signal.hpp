#ifndef _SIGNAL_H_
#define _SIGNAL_H_ 
#include <vector>
#include <numeric>
#include <cmath>
#include <fstream>

class Signal {
public:
    Signal(int samp_freq, int duration, int amplitude) 
        : m_sampFreq(samp_freq), m_duration(duration), m_amplitude(amplitude) {

        for (int i = 0; i < samp_freq * duration; ++i) {
            double time = static_cast<double>(i) / samp_freq;
            double mixed_signal = amplitude * sin(2 * M_PI * 100 * time);
            mixed_signal += amplitude * 0.3 * sin(2 * M_PI * 200 * time);
            mixed_signal += amplitude * 0.2 * sin(2 * M_PI * 400 * time);
            mixed_signal += amplitude * 0.1 * sin(2 * M_PI * 800 * time);
            mixed_signal += amplitude * 0.5 * sin(2 * M_PI * 900 * time);
            mixedSignal.push_back(mixed_signal);
        }
    }
    void generateTestSignal(std::vector<double> &signal, int samp_freq, int duration, int amplitude);
    std::vector<double> mixedSignal;
    std::vector<int> filteredSignal;
    void writeMixedSignal();
    void writeFilteredSignal();
private:
    int m_sampFreq;
    int m_duration;
    int m_amplitude;
};

#endif