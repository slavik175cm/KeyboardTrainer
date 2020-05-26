#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.resize(1920, 1080);
    w.move(0, 0);
    w.showMaximized();
    w.setWindowIcon(QIcon("KeyboardTrainerIcon.png")); //d:/qt_projects/KeyboardTrainer/KeyboardTrainerIcon2.png
    w.setWindowTitle("Keyboard Trainer");
    return a.exec();
}
