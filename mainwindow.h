#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextCursor>
#include <QTime>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    QTextCursor *a;
    Ui::MainWindow *ui;
    int current = 0;
    bool cursor_visible = 1;
    QString text;
    int len;
    qint64 time_started = 0;
    void keyPressEvent(QKeyEvent *event);
    void timerEvent(QTimerEvent *event);
    void change_letter_color(int pos, QString color);
    void change_letter_background(int pos, QString color);
    void add_html(QString &text);
    void restart();
    const QVector<int> available_symbols = {'.', ',', ';', '(', ')', ' ', '"'};

};
#endif // MAINWINDOW_H
