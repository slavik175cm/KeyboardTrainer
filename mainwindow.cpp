#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QKeyEvent>
#include <Qtimer>
#include <QMessageBox>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->text->setFocusPolicy(Qt::NoFocus);
    this->setStyleSheet("background-color: #4D535E;");
    ui->text->setStyleSheet("background-color: #4D535E;");
    ui->text->setFrameStyle(QFrame::NoFrame);
    ui->text->setStyleSheet("font: 25pt");
    ui->text->setTextInteractionFlags(Qt::TextInteractionFlag::NoTextInteraction);
    text = "The background of an element is the total size of the element, including padding and border (but not the margin).";
    len = text.size();
    add_html(text);

    ui->text->setText(text);
    startTimer(300);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::add_html(QString &text) {
    QString new_text = "";
    for (int i = 0; i < text.size(); i++) {
        new_text += "<font color=#9CA7BE>";
        new_text += "<span style=\"background-color: #4D535E\">";
        new_text += text[i];
    }
    text = new_text;
}

void MainWindow::change_letter_color(int pos, QString color) {
    int realpos = pos  * 61 + 12;
    text.remove(realpos, 7);
    text.insert(realpos, color);
    ui->text->setText(text);
}

void MainWindow::change_letter_background(int pos, QString color) {
    int realpos = pos  * 61 + 20 + 31;
    text.remove(realpos, 7);
    text.insert(realpos, color);
    ui->text->setText(text);
}

void MainWindow::restart() {
    time_started = 0;
    for (int i = 0; i < len; i++) {
        change_letter_color(i, "#9CA7BE");
        change_letter_background(i, "#4D535E");
    }
    current = 0;
    cursor_visible = 1;
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
    static bool wrong_letter = 0;
    if (event->key() == Qt::Key_Escape) {
        restart();
        wrong_letter = 0;
        return;
    }
    int c = event->key();
    if ((c < 'A' || c > 'Z') && !available_symbols.contains(c)) return;

    if (time_started == 0) {
        time_started = QDateTime::currentMSecsSinceEpoch();
    }
    if (event->modifiers() != Qt::ShiftModifier)
        c = tolower(c);
    if (ui->text->document()->toPlainText()[current] == c) {
        change_letter_background(current, "#4D535E");
        change_letter_color(current, wrong_letter ?  "#FA4940" : "#BBA8AD");
        wrong_letter = 0;
        current++;

        qDebug() << "OK";
        if (current == len) {
            QMessageBox::information(nullptr, "Ваш результат", "Символов в минуту: " + QString::number((qint64)len * 1000 * 60 / (QDateTime::currentMSecsSinceEpoch() - time_started)), 0, NULL);
            restart();
            return;
        }
        change_letter_background(current, "#48444A");
    } else {
        wrong_letter = 1;
        qDebug() << "WRONG";
    }
}

void MainWindow::timerEvent(QTimerEvent *event) {
    if (current == len) return;
    change_letter_background(current, cursor_visible ? "#48444A" : "#4D535E");
    cursor_visible ^= 1;
}

