#include "../includes/MainWindow.hpp"
#ifdef GUI
#include <QVBoxLayout>
#include <QWidget>
#include <QGroupBox>
#include <QPushButton>
#include <QHBoxLayout>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QWidget *central = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(central);

    // Khởi tạo audio output mặc định
    audioOutput = new QAudioOutput(this);
    audioOutput->setVolume(1.0);

    // ==== Playback Controls ====
    QGroupBox *playbackGroupBox = new QGroupBox("Playback Controls", central);
    QHBoxLayout *playbackLayout = new QHBoxLayout;

    auto makeButton = [&](const QString &text, auto slotFunc) {
        QPushButton *btn = new QPushButton(text);
        connect(btn, &QPushButton::clicked, this, slotFunc);
        playbackLayout->addWidget(btn);
        return btn;
    };

    mediaButton = std::unique_ptr<QPushButton>(makeButton("Play Media", &MainWindow::playMedia));
    voiceButton = std::unique_ptr<QPushButton>(makeButton("Play Voice", &MainWindow::playVoice));
    assistButton = std::unique_ptr<QPushButton>(makeButton("Play Assist", &MainWindow::playAssist));
    alarmButton = std::unique_ptr<QPushButton>(makeButton("Play Alarm", &MainWindow::playAlarm));
    notificationButton = std::unique_ptr<QPushButton>(makeButton("Play Notification", &MainWindow::playNotif));

    // Nút Stop
    stopButton = std::unique_ptr<QPushButton>(makeButton("Stop", &MainWindow::stopPlayback));

    playbackGroupBox->setLayout(playbackLayout);
    mainLayout->addWidget(playbackGroupBox);

    // ==== Equalizer ====
    QGroupBox *eqGroupBox = new QGroupBox("Equalizer", central);
    QHBoxLayout *eqLayout = new QHBoxLayout;

    auto makeDial = [&](const QString &labelText) {
        QVBoxLayout *lay = new QVBoxLayout;
        QDial *dial = new QDial;
        QLabel *lab = new QLabel(labelText);
        lab->setAlignment(Qt::AlignCenter);
        lay->addWidget(dial);
        lay->addWidget(lab);
        eqLayout->addLayout(lay);
        return dial;
    };

    lowDial = std::unique_ptr<QDial>(makeDial("LOW"));
    midDial = std::unique_ptr<QDial>(makeDial("MID"));
    trebleDial = std::unique_ptr<QDial>(makeDial("TREBLE"));

    eqGroupBox->setLayout(eqLayout);
    mainLayout->addWidget(eqGroupBox);

    // ==== Volume ====
    QVBoxLayout *volLayout = new QVBoxLayout;
    volumeDial = std::unique_ptr<QDial>(new QDial);
    QLabel *volLabel = new QLabel("VOLUME");
    volLabel->setAlignment(Qt::AlignCenter);
    volLayout->addWidget(volumeDial.get());
    volLayout->addWidget(volLabel);
    mainLayout->addLayout(volLayout);

    central->setLayout(mainLayout);
    setCentralWidget(central);
}

// ==== Helper function to play sound ====
void MainWindow::playSound(const QString &filePath) {
    QMediaPlayer *newPlayer = new QMediaPlayer(this);
    QAudioOutput *newAudioOutput = new QAudioOutput(this);

    newPlayer->setAudioOutput(newAudioOutput);
    newAudioOutput->setVolume(1.0); 

    newPlayer->setSource(QUrl::fromLocalFile(filePath));

    connect(newPlayer, &QMediaPlayer::mediaStatusChanged, this, [newPlayer, newAudioOutput](QMediaPlayer::MediaStatus status) {
        if (status == QMediaPlayer::EndOfMedia) {
            newPlayer->deleteLater();
            newAudioOutput->deleteLater();
        }
    });
    newPlayer->play();
}

// ==== Slot implementations ====
void MainWindow::playMedia() {
    playSound("../../sounds/media.wav");
}

void MainWindow::playVoice() {
    playSound("../../sounds/voicephone.wav");
}

void MainWindow::playAssist() {
    playSound("../../sounds/assistance.wav");
}

void MainWindow::playAlarm() {
    playSound("../../sounds/alarm.wav");
}

void MainWindow::playNotif() {
    playSound("../../sounds/notif.wav");
}

void MainWindow::stopPlayback() {
    for (auto player : findChildren<QMediaPlayer*>()) {
        player->stop();
        player->deleteLater();
    }
}
#endif