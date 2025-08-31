#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <QSlider>
#include <QDial>
#include <QLabel>
#include <QAudioOutput>
#include <QGroupBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QMediaPlayer>
#include <QHBoxLayout>
#include <memory>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

private slots:
    void playMedia();
    void playVoice();
    void playAssist();
    void playAlarm();
    void playNotif();
    void stopPlayback(); 

private:
    QMediaPlayer *player = nullptr;
    QAudioOutput *audioOutput = nullptr;

    std::unique_ptr<QPushButton> mediaButton;
    std::unique_ptr<QPushButton> voiceButton;
    std::unique_ptr<QPushButton> assistButton;
    std::unique_ptr<QPushButton> alarmButton;
    std::unique_ptr<QPushButton> notificationButton;
    std::unique_ptr<QPushButton> stopButton;

    std::unique_ptr<QDial> lowDial;
    std::unique_ptr<QDial> midDial;
    std::unique_ptr<QDial> trebleDial;
    std::unique_ptr<QDial> volumeDial;

    void playSound(const QString &filePath);
};

#endif