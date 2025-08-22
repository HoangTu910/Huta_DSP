/*
    Author: HuTao
    Implementation of some digital signal processing filters in C++
    All the computational was done using Q17.14 format 
*/

#include "../includes/module/CanonicalFilter.hpp"
#include "../includes/module/StateVariableFilter.hpp"
#include "../includes/module/ShelvingFilter.hpp"
#include "../includes/test/Signal.hpp"
#include "../includes/common/filterType.hpp"

#include <memory>

using namespace std;

/* Some parameter */
const int SAMP_FREQ = 44100; 
const int CUTOFF_FREQ = 800;
const int CENTER_FREQ = 800;
const double Q_FACTOR = 0.707;
const int SIGNAL_DURATION = 1;
const double SIGNAL_AMPLITUDE = 0.06;
const double GAIN_FACTOR = 20.0; // dB
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

    signal->writeSignal(signal->t_inputSignal, TestFiles::Input);
    signal->writeSignal(signal->t_outputSignal[1], TestFiles::Output1);

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
}