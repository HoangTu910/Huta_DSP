/*
    Author: HuTao
    Implementation of some digital signal processing filters in C++
    All the computational was done using Q16.15 format 
*/

#include "../includes/test/TestObject.hpp"
#include "../includes/config/AudioManager.hpp"
#include <sndfile.h>
#include <memory>

#ifdef GUI
#include "../gui/includes/MainWindow.hpp"
#include <QApplication>
#endif

using namespace std;

int main(int argc, char *argv[]) {
    #ifdef GUI
    QApplication app(argc, argv);
    #endif
    /*
        TODO: Triển khai theo kiểu từng frame. Cứ viết hàm process cho mỗi modules
        Sau đó với 1 input sẽ cắt ra thành mỗi buffer. Các buffer sẽ được đưa vào
        modules rồi xử lý riêng biệt
        Ex:
        Dưới đây là một file âm thanh
        |========================================================|
        Áp dụng frame count, mỗi frame cho là dài 2ms, tạo JSON để quyết định tại frame đó
        thì áp dụng gì. Chia frame ví dụ như sau:
        |=====(Frame 1)=====|======(Frame 2)======|==============|
        Tất cả đều áp dụng trên một file input. Có nghĩa là truyền tham chiếu buffer đó vào 
        hàm process hoặc tạo một buffer copy rồi xử lý buffer copy đó, sau đó cho ra output.
    */
    unique_ptr<Test::TestObject> pTest(new Test::TestObject());
    pTest->runAllCoreProcessing();

    #ifdef GUI
    unique_ptr<AudioManager> audioManager(new AudioManager());
    audioManager->init();

    std::cout << "Running Application...\n";
    MainWindow mainWindow;
    mainWindow.show();

    return app.exec();
    #endif
}
