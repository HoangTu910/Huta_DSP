/*
    Author: HuTao
    Implementation of some digital signal processing filters in C++
    All the computational was done using Q16.15 format 
*/

#include "../includes/core/CanonicalFilter.hpp"
#include "../includes/core/BiquadFilter.hpp"
#include "../includes/core/StateVariableFilter.hpp"
#include "../includes/core/ShelvingFilter.hpp"
#include "../includes/core/PeakFilter.hpp"
#include "../includes/test/Signal.hpp"
#include "../includes/common/filterType.hpp"
#include "../includes/core/ParametricEqualizer.hpp"
#include "../includes/test/TestObject.hpp"
#include "../gui/includes/MainWindow.hpp"
#include <sndfile.h>
#include <memory>
#include <QApplication>

using namespace std;

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    unique_ptr<Test::TestObject> pTest(new Test::TestObject());
    pTest->runAllCoreProcessing();

    std::cout << "Running Application...\n";
    MainWindow mainWindow;
    mainWindow.show();

    return app.exec();
}
