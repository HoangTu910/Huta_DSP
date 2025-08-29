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
#define CANONICAL_BOOST_MODE 14
#define CANONICAL_CUT_MODE 15
#define PEAK_BOOST_MODE 16
#define PEAK_CUT_MODE 17

#define HIGH_SHELF 123
#define LOW_SHELF 321

const double PI_DOUBLE = 3.14159265358979323846;
const int SCALE_FACTOR = 2;

/* Some parameter */
const int SAMP_FREQ = 44100; 
const int CUTOFF_FREQ = 200;
const int CENTER_FREQ = 800;
const double Q_FACTOR = 0.707;
const int SIGNAL_DURATION = 2;
const double SIGNAL_AMPLITUDE = 0.06;
const double GAIN_FACTOR = 20.0; // dB
const double GAIN_EQUALIZER = 2.0;
const double any = 1.0;
/* Some parameter */

const int BASS_LOUDNESS_COMPENSATION = 150;
const int TREBLE_LOUDNESS_COMPENSATION = 150;

enum class Type {
    LowPass,
    HighPass,
    BandPass_Q,
    BandPass_Constant,
    Notch, // aka band-stop
    AllPass,
    Peaking,
    LowShelf,
    HighShelf
};

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

namespace SoundTestFiles {
    inline const std::string Output1 = "outputSound_1.wav";
    inline const std::string Output2 = "outputSound_2.wav";
    inline const std::string Output3 = "outputSound_3.wav";
    inline const std::string Output4 = "outputSound_4.wav";
    inline const std::string Output5 = "outputSound_5.wav";
    inline const std::string Output6 = "outputSound_6.wav";
    inline const std::string Output7 = "outputSound_7.wav";
    inline const std::string Output8 = "outputSound_8.wav";
    inline const std::string Output9 = "outputSound_9.wav";
    inline const std::string Output10 = "outputSound_10.wav";
    inline const std::string Input  = "inputSound.wav";
}

#endif // FILTER_TYPE_HPP