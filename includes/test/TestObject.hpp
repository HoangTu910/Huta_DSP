#ifndef _TEST_OBJECT_H
#define _TEST_OBJECT_H

#include "../includes/core/CanonicalFilter.hpp"
#include "../includes/core/BiquadFilter.hpp"
#include "../includes/core/StateVariableFilter.hpp"
#include "../includes/core/ShelvingFilter.hpp"
#include "../includes/core/PeakFilter.hpp"
#include "../includes/test/Signal.hpp"
#include "../includes/common/filterType.hpp"
#include "../includes/core/ParametricEqualizer.hpp"
#include "../includes/modules/Limiter.hpp"
#include <sndfile.h>
#include <memory>

namespace Test {
    class TestObject {
    public:
        void runAllCoreProcessing();
    };
}


#endif