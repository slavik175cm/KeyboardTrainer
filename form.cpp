#include "form.h"
#include "ui_form.h"
#include <QDebug>
#include <QMessageBox>
Form::Form(MainWindow *mainwindow, DataBase *db, User *user, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form)
{
    ui->setupUi(this);
    this->mainwindow = mainwindow;
    this->db = db;
    this->user = user;
    ui->hideButton->setChecked(true);
    ui->password_field->setEchoMode(QLineEdit::Password);
    setWindowFlags(Qt::WindowStaysOnTopHint);
}

Form::~Form()
{
    delete ui;
}

void Form::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Escape)
        on_cancel_button_clicked();

    if (event->key() == Qt::Key_Return)
        on_OK_button_clicked();

}

void Form::show(int mode) {
    this->mode = mode;
    if (mode == LogIn) ui->mode_label->setText("Log in");
        else ui->mode_label->setText("Sign up");
    QWidget::show();
    ui->username_field->setFocus();
}

void Form::on_OK_button_clicked()
{
    if (ui->username_field->text().isEmpty()) {
        QMessageBox::critical(this, "Error", "Username field can't be empty");
        return;
    }
    if (ui->password_field->text().isEmpty()) {
        QMessageBox::critical(this, "Error", "Password field can't be empty");
        return;
    }
    if (mode == SignUp) {
        if (db->is_name_exist(ui->username_field->text())) {
            QMessageBox::critical(this, "Error", "Such username already exists");
            return;
        }
        *user = *(new User(user->db));
        user->name = ui->username_field->text();
        user->password = ui->password_field->text();
        db->add_user(user);
        on_cancel_button_clicked();
        mainwindow->on_user_changed();
        QMessageBox::information(this, "Success", "Profile has been created!\nHello " + user->name);
    }
    if (mode == LogIn) {
        if (!db->is_name_exist(ui->username_field->text())) {
            QMessageBox::critical(this, "Error", "Such username doesn't exist");
            return;
        }
        if (!db->is_password_correct(ui->username_field->text(), ui->password_field->text())) {
            QMessageBox::critical(this, "Error", "Password is wrong");
            return;
        }
        *user = *db->get_user(ui->username_field->text());
        on_cancel_button_clicked();
        mainwindow->on_user_changed();
        QMessageBox::information(this, "Success", "Hello " + user->name);
    }
}

void Form::closeEvent(QCloseEvent *event) {
    on_cancel_button_clicked();
}

void Form::on_cancel_button_clicked()
{
    mainwindow->setEnabled(true);
    ui->username_field->setText("");
    ui->password_field->setText("");
    this->close();
}

void Form::on_hideButton_clicked()
{
    if (ui->hideButton->isChecked())
        ui->password_field->setEchoMode(QLineEdit::Password);
    else ui->password_field->setEchoMode(QLineEdit::EchoMode::Normal);
}

