#ifndef _FILTER_H_ 
#define _FILTER_H_

#include <iostream>
#include <vector>
#include <cmath>
#include <numeric>

const int Q15_PI = 102944;

class IFilter { 
public:
    virtual int process(int input) = 0;
    virtual void process(std::vector<double> &inputSignal, std::vector<int> &outputSignal) = 0;
    virtual void normalize(std::vector<double> &coeff);
};

#endif