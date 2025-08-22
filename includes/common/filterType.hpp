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
#define LF_BOOST 9
#define LF_CUT 10
#define HF_BOOST 11
#define HF_CUT 12

const double PI_DOUBLE = 3.14159265358979323846;
const int SCALE_FACTOR = 2;

/* I defined 10 files for signal testing, you can increase it if needed ^^ */

namespace TestFiles {
    inline const std::string Output1 = "outputSignal_1.txt";
    inline const std::string Output2 = "outputSignal_2.txt";
    inline const std::string Output3 = "outputSignal_3.txt";
    inline const std::string Output4 = "outputSignal_4.txt";
    inline const std::string Output5 = "outputSignal_5.txt";
    inline const std::string Output6 = "outputSignal_6.txt";
    inline const std::string Output7 = "outputSignal_7.txt";
    inline const std::string Output8 = "outputSignal_8.txt";
    inline const std::string Output9 = "outputSignal_9.txt";
    inline const std::string Output10 = "outputSignal_10.txt";
    inline const std::string Input  = "inputSignal.txt";
}


#endif // FILTER_TYPE_HPP