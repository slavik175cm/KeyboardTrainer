#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QKeyEvent>
#include <QMessageBox>
#include <QtMath>
#include <QFileDialog>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->text->setFocusPolicy(Qt::NoFocus);
    ui->spm->setFocusPolicy(Qt::NoFocus);
    ui->random_text_button->setFocusPolicy(Qt::NoFocus);
    ui->custom_text_button->setFocusPolicy(Qt::NoFocus);
    ui->wrong_symbols->setFocusPolicy(Qt::NoFocus);
    ui->custom_text->setFocusPolicy(Qt::ClickFocus);
    ui->length_slider->setFocusPolicy(Qt::NoFocus);

    ui->custom_text->hide();
    ui->length_slider->hide();

    this->setStyleSheet("background-color: #4D535E;");
    ui->text->setStyleSheet("background-color: #4D535E;");
    ui->text->setFrameStyle(QFrame::NoFrame);
    ui->text->setFont(QFont("Comic Sans MC", 25));
    ui->text->setTextInteractionFlags(Qt::TextInteractionFlag::NoTextInteraction);
    ui->text->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    ui->spm->setFont(QFont("Comic Sans MC", 40));
    ui->spm->setTextInteractionFlags(Qt::TextInteractionFlag::NoTextInteraction);
    ui->spm->setFrameStyle(QFrame::NoFrame);
    ui->spm->setText("<p align=\"center\"><font color=#BBA8AD>-");

    ui->wrong_symbols->setFont(QFont("Comic Sans MC", 40));
    ui->wrong_symbols->setTextInteractionFlags(Qt::TextInteractionFlag::NoTextInteraction);
    ui->wrong_symbols->setFrameStyle(QFrame::NoFrame);
    ui->wrong_symbols->setText("<p align=\"center\"><font color=#FF4500>-");

    myFont = new QFont(ui->text->font().family(), ui->text->font().pointSize());
    fm = new QFontMetrics(*myFont);

    read_frequency_table();

    //text = "The background of an element is the total size of the element, including padding and border (but not the margin).";
    //set_text(text);
    ui->length_slider->setValue(100);
    set_text(renerate_random_text(100));

    startTimer(300);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::next_block() {
    current = 0;
    int taken_rows = 0;
    block = "";
    QString cur = "";
    for (int i = last_not_taken; i < list_of_words.size(); i++) {
        cur += list_of_words[i] + ' ';
        if (i == list_of_words.size() - 1 || (fm->horizontalAdvance(cur + list_of_words[i + 1]) > ui->text->width() - 10)) {
            last_not_taken = i + 1;
            taken_rows++;
            block += cur;
            cur = "";
            if (i == list_of_words.size() - 1 || taken_rows == block_rows) {
                block.remove(block.size() - 1, 1);
                block_len = block.size();
                add_html(block);
                ui->text->setText(block);
                return;
            }
        }
    }
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
    block.remove(realpos, 7);
    block.insert(realpos, color);
    ui->text->setText(block);
}

void MainWindow::change_letter_background(int pos, QString color) {
    int realpos = pos  * 61 + 20 + 31;
    block.remove(realpos, 7);
    block.insert(realpos, color);
    ui->text->setText(block);
}

void MainWindow::restart() {
    last_not_taken = 0;
    time_started = 0;
    cursor_visible = 1;
    wrong_symbols = 0;
    next_block();
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
    if (event->modifiers() != Qt::ShiftModifier)
        c = tolower(c);

    if (time_started == 0)
        time_started = QDateTime::currentMSecsSinceEpoch();

    if (ui->text->document()->toPlainText()[current] == c) {
        change_letter_background(current, "#4D535E");
        change_letter_color(current, wrong_letter ?  "#FA4940" : "#BBA8AD");
        wrong_letter = 0;
        current++;

        if (current == block_len && last_not_taken == list_of_words.size()) {
            ui->spm->setText("<p align=\"center\"><font color=#BBA8AD>" + QString::number((qint64)text.size() * 1000 * 60 / (QDateTime::currentMSecsSinceEpoch() - time_started)));
            ui->wrong_symbols->setText("<p align=\"center\"><font color=#FF4500>" + QString::number(wrong_symbols));
            restart();
            return;
        }
        if (current == block_len) {
            next_block();
            return;
        }
        change_letter_background(current, "#48444A");
    } else {
        wrong_letter = 1;
        wrong_symbols++;
    }
}

void MainWindow::timerEvent(QTimerEvent *event) {
    if (current == block_len) return;
    change_letter_background(current, cursor_visible ? "#48444A" : "#4D535E");
    cursor_visible ^= 1;
}

void MainWindow::mousePressEvent(QMouseEvent *event) {
    if (this->focusWidget() != NULL) this->focusWidget()->clearFocus();
}

void MainWindow::set_text(QString new_text) {
    text = new_text;
    list_of_words = text.split(' ');
    block_rows = (ui->text->height() / fm->boundingRect(text).height());
    restart();
}

void MainWindow::on_custom_text_textChanged() {
    if (ui->custom_text->toPlainText().size() == 0) {
        QMessageBox::warning(NULL, "error", "это поле не должно быть пустым");
        return;
    }
    set_text(ui->custom_text->toPlainText());
}

void MainWindow::read_frequency_table() {
    QFile file("D:\\qt_projects\\KeyboardTrainer\\FrequencyTable.txt");
    file.open(QIODevice::ReadWrite);
    for (int i = 0; i < 26; i++) {
        QStringList line = QString(file.readLine()).split(' ');
        for (int j = 0; j < 26; j++)
            frequency_table[i][j] = line[j].toInt();
    }
}

char MainWindow::next_letter(QChar ch) {
    int sum = 0;
    for (int i = 0; i < 26; i++)
        sum += frequency_table[ch.unicode() - 'a'][i];
    int rnd = rand() % sum;
    sum = 0;
    for (int i = 0; i < 26; i++) {
        sum += frequency_table[ch.unicode() - 'a'][i];
        if (sum >= rnd) return 'a' + i;
    }
}

QString MainWindow::renerate_random_text(int len) {
    QString text = "";
    while (text.size() < len) {
        int word_len = qMin(rand() % 6 + 1, len - text.size());
        QString word = QString('a' + rand() % 26);
        for (int i = 0; i < word_len; i++)
            word += next_letter(word[i]);
        text += word;
        if (text.size() < len) text += ' ';
    }
    return text;
}

void MainWindow::on_length_slider_valueChanged(int value) {
    set_text(renerate_random_text(value));
}

void MainWindow::on_custom_text_button_clicked() {
    ui->custom_text->show();
    ui->length_slider->hide();
}

void MainWindow::on_random_text_button_clicked() {
    ui->length_slider->show();
    ui->custom_text->hide();
}
