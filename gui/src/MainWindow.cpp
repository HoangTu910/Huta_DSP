#include "../includes/MainWindow.hpp"
#include <QVBoxLayout>
#include <QWidget>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // Widget trung tâm
    QWidget *central = new QWidget(this);
    setCentralWidget(central);

    // Label
    label = new QLabel("Hello Qt + CMake + MinGW!", this);
    label->setAlignment(Qt::AlignCenter);

    // Button
    button = new QPushButton("Click me", this);

    // Layout
    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(label);
    layout->addWidget(button);
    central->setLayout(layout);

    // Connect signal-slot
    connect(button, &QPushButton::clicked, this, &MainWindow::onButtonClicked);

    // Set window title
    setWindowTitle("Qt Test Window");
    resize(400, 200);
}

void MainWindow::onButtonClicked()
{
    label->setText("Button clicked!");
}
