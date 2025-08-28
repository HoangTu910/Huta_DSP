#include "../../includes/test/Signal.hpp"
#include "../../includes/core/CanonicalFilter.hpp"

void Signal::generateTestSignal(std::vector<double> &signal, int samp_freq, int duration, int amplitude)
{
    for (int i = 0; i < samp_freq * duration; ++i) {
        double time = static_cast<double>(i) / samp_freq;
        double low_freq_signal = amplitude * sin(2 * M_PI * 100 * time);
        double high_freq_noise = amplitude * 0.5 * sin(2 * M_PI * 800 * time);
        double mixed_signal = low_freq_signal + high_freq_noise;
        signal.push_back(mixed_signal);
    }
}

void Signal::writeMixedSignal()
{
    std::ofstream outFile("inputSignal.txt");
    if (outFile.is_open()) {
        for (size_t i = 0; i < this->t_inputSignal.size(); ++i) {
            outFile << i / static_cast<double>(m_sampFreq) << " " << this->t_inputSignal[i] << "\n";
        }
        outFile.close();
        std::cout << "Write done mixedSignal.txt" << std::endl;
    } else {
        std::cout << "Error!" << std::endl;
    }
}

void Signal::writeFilteredSignal()
{
    std::ofstream outFile("filteredSignal.txt");
    if (outFile.is_open()) {
        for (size_t i = 0; i < this->t_inputSignal.size(); ++i) {
            outFile << i / static_cast<double>(m_sampFreq) << " " << this->t_inputSignal[i] << "\n";
        }
        outFile.close();
        std::cout << "Write done filteredSignal.txt" << std::endl;
    } else {
        std::cout << "Error!" << std::endl;
    }
}

