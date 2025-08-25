/*
    Author: HuTao
    Implementation of some digital signal processing filters in C++
    All the computational was done using Q16.15 format 
*/

#include "../includes/module/CanonicalFilter.hpp"
#include "../includes/module/BiquadFilter.hpp"
#include "../includes/module/StateVariableFilter.hpp"
#include "../includes/module/ShelvingFilter.hpp"
#include "../includes/module/PeakFilter.hpp"
#include "../includes/test/Signal.hpp"
#include "../includes/common/filterType.hpp"
#include <sndfile.h>

#include <memory>

using namespace std;

/* Some parameter */
const int SAMP_FREQ = 44100; 
const int CUTOFF_FREQ = 200;
const int CENTER_FREQ = 800;
const double Q_FACTOR = 0.707;
const int SIGNAL_DURATION = 2;
const double SIGNAL_AMPLITUDE = 0.06;
const double GAIN_FACTOR = 20.0; // dB
const double any = 1.0;
/* Some parameter */

int main() {
    /* 
        * When create signal object, it will automatically gerenate signal base on parameter below
        * You can adjust the signal by adjust the constructor of Signal class 
    */
    unique_ptr<Signal> signal(new Signal(SAMP_FREQ, SIGNAL_DURATION, SIGNAL_AMPLITUDE)); 
    
    /* 
        * t_outputSignal is a 2D vector, each row is a filtered signal.
        * If you don't understand 2D vector, you can just take the output (.txt) in /build for visualization. 
        * I output the signals into separate .txt files in the /build directory, 
          you can use that and do anything with it.
    */

    /* Canonical testing */
    unique_ptr<Filter::CanonicalFilter> canonicalFilter(new Filter::CanonicalFilter(SAMP_FREQ));

    canonicalFilter->setType(LOW_PASS_2, CUTOFF_FREQ, Q_FACTOR);
    canonicalFilter->process(signal->t_inputSignal, signal->t_outputSignal[1]);

    /* Write signal into .txt and plot it out using python in visualize/ */
    signal->writeSignal(signal->t_inputSignal, TestFiles::Input);
    signal->writeSignal(signal->t_outputSignal[1], TestFiles::Output1);

    /* We can hear the sound of the signal by using createSoundSignal() method
       Remember to change the namespace from TestFiles to SoundTestFiles    */
    signal->createSoundSignal(signal->t_outputSignal[1], SoundTestFiles::Output1);

    /* SVF testing */
    unique_ptr<Filter::StateVariableFilter> svf(new Filter::StateVariableFilter(SAMP_FREQ));
    svf->tuning(CUTOFF_FREQ, Q_FACTOR);
    svf->process(signal->t_inputSignal, signal->t_outputSignal[2], 
                signal->t_outputSignal[3], signal->t_outputSignal[4]); // three outputs: hpass, bpass, lpass
    signal->writeSignal(signal->t_outputSignal[2], TestFiles::Output2);
    signal->writeSignal(signal->t_outputSignal[3], TestFiles::Output3);
    signal->writeSignal(signal->t_outputSignal[4], TestFiles::Output4);

    /* Shelving testing */
    unique_ptr<Filter::ShelvingFilter> shelvingFilter(new Filter::ShelvingFilter(SAMP_FREQ));
    shelvingFilter->setType(LF_BOOST, CUTOFF_FREQ, GAIN_FACTOR);
    shelvingFilter->process(signal->t_inputSignal, signal->t_outputSignal[5]);
    signal->writeSignal(signal->t_outputSignal[5], TestFiles::Output5);
    signal->createSoundSignal(signal->t_outputSignal[5], SoundTestFiles::Output5);

    /* Peak testing */
    unique_ptr<Filter::PeakFilter> peakFilter(new Filter::PeakFilter(SAMP_FREQ));

    /* The set type in peak filter allow you to choose between Canonical implement or direct different equation implement */
    peakFilter->setType(PEAK_BOOST_MODE, CUTOFF_FREQ, GAIN_FACTOR, Q_FACTOR);
    peakFilter->peakProcess(signal->t_inputSignal, signal->t_outputSignal[6]); // please notice that here I used peakProcess(), not process()!
    signal->writeSignal(signal->t_outputSignal[6], TestFiles::Output6);
    signal->createSoundSignal(signal->t_outputSignal[6], SoundTestFiles::Output6);

    unique_ptr<Filter::BiquadFilter> biquadFilter(new Filter::BiquadFilter(SAMP_FREQ));
    biquadFilter->setParameters(Type::LowPass, CUTOFF_FREQ, Q_FACTOR, any);
    biquadFilter->process(signal->t_inputSignal, signal->t_outputSignal[7]);
    signal->writeSignal(signal->t_outputSignal[7], TestFiles::Output7);
    signal->createSoundSignal(signal->t_outputSignal[7], SoundTestFiles::Output7);
}