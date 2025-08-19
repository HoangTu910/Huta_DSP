#include "../includes/module/CanonicalFilter.hpp"
#include "../includes/module/Signal.hpp"

#include <memory>

using namespace std;

const int SAMP_FREQ = 2000; 
const int CUTOFF_FREQ = 200;

int main() {
    unique_ptr<Signal> signal(new Signal(SAMP_FREQ, 1, 1));
    unique_ptr<CanonicalFilter> canonicalFilter(new CanonicalFilter(SAMP_FREQ));

    canonicalFilter->setType(HIGH_PASS_2, CUTOFF_FREQ);
    canonicalFilter->process(signal->mixedSignal, signal->filteredSignal);

    signal->writeFilteredSignal();
    signal->writeMixedSignal();
}