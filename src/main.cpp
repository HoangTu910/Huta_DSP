/*
    Author: HuTao
    Implementation of some digital signal processing filters in C++
    All the computational was done using Q16.15 format 
*/

#include "../includes/test/TestObject.hpp"
#include "../gui/includes/MainWindow.hpp"
#include "../includes/config/AudioManager.hpp"
#include <sndfile.h>
#include <memory>
#include <QApplication>

using namespace std;

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    unique_ptr<Test::TestObject> pTest(new Test::TestObject());
    pTest->runAllCoreProcessing();

    unique_ptr<AudioManager> audioManager(new AudioManager());
    audioManager->init();

    std::cout << "Running Application...\n";
    MainWindow mainWindow;
    mainWindow.show();

    return app.exec();
}
