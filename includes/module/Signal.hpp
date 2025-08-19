#ifndef _SIGNAL_H_
#define _SIGNAL_H_ 

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

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
            t_inputSignal.push_back(mixed_signal);
        }
    }
    void generateTestSignal(std::vector<double> &signal, int samp_freq, int duration, int amplitude);
    
    /*I created many buffer for testing, can be removed to reduce memory used*/
    std::vector<double> t_inputSignal;
    std::vector<int> t_outputSignal_1;
    std::vector<int> t_outputSignal_2;
    std::vector<int> t_outputSignal_3;
    std::vector<int> t_outputSignal_4;
    std::vector<int> t_outputSignal_5;

    void writeMixedSignal();
    void writeFilteredSignal();
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
private:
    int m_sampFreq;
    int m_duration;
    int m_amplitude;
};

#endif
