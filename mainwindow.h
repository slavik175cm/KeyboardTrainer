#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextCursor>
#include <QTime>
#include "keyboard.h"
#include "textgenerator.h"
#include "textprovider.h"
#include "settingsbar.h"
#include "statisticsprovider.h"
#include "user.h"
#include "database.h"
#include "form.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE
class SettingsBar;
class StatisticsProvider;
class Form;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void on_user_changed();

private slots:
    void on_custom_text_button_clicked();

    void on_custom_text_textChanged();

    void on_length_slider_valueChanged(int value);

    void on_random_text_button_clicked();


private:
    QGraphicsScene *scene;
    QGraphicsView *view;
    Keyboard *keyboard;
    TextGenerator *text_generator;
    TextProvider *text_provider;
    SettingsBar *settings_bar;
    StatisticsProvider *statistics_provider;
    User *user;
    DataBase *database;
    Form *form;
    Ui::MainWindow *ui;

    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    void mousePressEvent(QMouseEvent *event);
};
#endif // MAINWINDOW_H
