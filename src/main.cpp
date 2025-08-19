#include "../includes/module/CanonicalFilter.hpp"
#include "../includes/module/Signal.hpp"
#include "../includes/common/filterType.h"

#include <memory>

using namespace std;

const int SAMP_FREQ = 3000; 
const int CUTOFF_FREQ = 50;
const int CENTER_FREQ = 910;
const double Q_FACTOR = 0.707;
const int SIGNAL_DURATION = 1;
const int SIGNAL_AMPLITUDE = 1;

int main() {
    unique_ptr<Signal> signal(new Signal(SAMP_FREQ, SIGNAL_DURATION, SIGNAL_AMPLITUDE));
    unique_ptr<Filter::CanonicalFilter> canonicalFilter(new Filter::CanonicalFilter(SAMP_FREQ));

    canonicalFilter->setType(HIGH_PASS_2, CENTER_FREQ, Q_FACTOR);
    canonicalFilter->process(signal->mixedSignal, signal->filteredSignal);

    signal->writeFilteredSignal();
    signal->writeMixedSignal();
}