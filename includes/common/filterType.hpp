#ifndef FILTER_TYPE_HPP
#define FILTER_TYPE_HPP

#include <string>

#define LOW_PASS_1 1
#define HIGH_PASS_1 2
#define ALL_PASS_1 3
#define LOW_PASS_2 4
#define HIGH_PASS_2 5
#define ALL_PASS_2 6
#define BAND_PASS_2 7
#define BAND_REJECT_2 8

namespace TestFiles {
    inline const std::string Output1 = "outputSignal_1.txt";
    inline const std::string Output2 = "outputSignal_2.txt";
    inline const std::string Output3 = "outputSignal_3.txt";
    inline const std::string Output4 = "outputSignal_4.txt";
    inline const std::string Output5 = "outputSignal_5.txt";
    inline const std::string Input  = "inputSignal.txt";
}


#endif // FILTER_TYPE_HPP