#ifndef FORM_H
#define FORM_H

#include <QWidget>
#include "mainwindow.h"
#include "database.h"
namespace Ui {
class Form;
}
class MainWindow;
class Form : public QWidget
{
    Q_OBJECT

public:
    explicit Form(MainWindow *mainwindow, DataBase *db, User *user, QWidget *parent = nullptr);
    ~Form();
    void show(int mode);
    void keyPressEvent(QKeyEvent *event);
    const static int LogIn = 1, SignUp = 2;
    int mode;

private slots:
    void on_OK_button_clicked();
    void closeEvent(QCloseEvent *event) override;

    void on_cancel_button_clicked();

    void on_hideButton_clicked();

private:
    Ui::Form *ui;
    MainWindow *mainwindow;
    DataBase *db;
    User *user;
};

#endif // FORM_H
