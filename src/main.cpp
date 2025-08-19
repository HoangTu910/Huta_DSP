#include "../includes/module/CanonicalFilter.hpp"
#include "../includes/module/StateVariableFilter.hpp"
#include "../includes/module/Signal.hpp"
#include "../includes/common/filterType.hpp"

#include <memory>

using namespace std;

const int SAMP_FREQ = 3000; 
const int CUTOFF_FREQ = 50;
const int CENTER_FREQ = 200;
const double Q_FACTOR = 0.707;
const int SIGNAL_DURATION = 1;
const int SIGNAL_AMPLITUDE = 1;

int main() {
    unique_ptr<Signal> signal(new Signal(SAMP_FREQ, SIGNAL_DURATION, SIGNAL_AMPLITUDE)); //test signal
    
    /* Canonical testing */
    unique_ptr<Filter::CanonicalFilter> canonicalFilter(new Filter::CanonicalFilter(SAMP_FREQ));

    canonicalFilter->setType(LOW_PASS_2, CENTER_FREQ, Q_FACTOR);
    canonicalFilter->process(signal->t_inputSignal, signal->t_outputSignal_1);

    signal->writeSignal(signal->t_inputSignal, TestFiles::Input);
    signal->writeSignal(signal->t_outputSignal_1, TestFiles::Output1);

    /* SVF testing */
    unique_ptr<Filter::StateVariableFilter> svf(new Filter::StateVariableFilter(SAMP_FREQ));
    svf->tuning(CENTER_FREQ, Q_FACTOR);
    svf->process(signal->t_inputSignal, signal->t_outputSignal_2, 
                signal->t_outputSignal_3, signal->t_outputSignal_4); // three outputs: hpass, bpass, lpass
    signal->writeSignal(signal->t_outputSignal_2, TestFiles::Output2);
    signal->writeSignal(signal->t_outputSignal_3, TestFiles::Output3);
    signal->writeSignal(signal->t_outputSignal_4, TestFiles::Output4);
}