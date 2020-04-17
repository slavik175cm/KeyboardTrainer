#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextCursor>
#include <QTime>
#include "keyboard.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_custom_text_button_clicked();

    void on_custom_text_textChanged();

    void on_length_slider_valueChanged(int value);

    void on_random_text_button_clicked();

private:
    Keyboard *keyboard;

    int frequency_table[26][26];
    QStringList list_of_words;
    QFont *myFont;
    QFontMetrics *fm;
    Ui::MainWindow *ui;
    int current = 0, current_block = 0;
    int wrong_symbols = 0;
    bool cursor_visible = 1;
    QString text, block;
    int block_len, blocksize, block_rows, last_not_taken = 0;
    qint64 time_started = 0;
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    void timerEvent(QTimerEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void change_letter_color(int pos, QString color);
    void change_letter_background(int pos, QString color);
    void read_frequency_table();
    char next_letter(QChar ch);
    QString generate_random_text(int len);
    char first_letter();
    void next_block();
    void add_html(QString &text);
    void set_text(QString new_text);
    void restart();
    const QVector<int> available_symbols = {'.', ',', ';', '(', ')', ' ', '"', '\''};
    const int first_letter_frequency[26] = {11682, 4434, 5238, 3174, 2799, 4027, 1642, 4200, 7294, 511, 856, 2415, 3826, 2284, 7631, 4319, 222, 2826, 6686, 15978, 1183, 824, 5497, 45, 763, 45}; // a..z
    const QVector<char> consonant = { 'B', 'C', 'D', 'F', 'G', 'H', 'J', 'K', 'L', 'M', 'N', 'P', 'Q', 'R', 'S', 'T', 'V', 'X', 'W', 'Z'};
    const QVector<char> vowels = { 'A', 'E', 'I', 'O', 'U', 'Y'};
};
#endif // MAINWINDOW_H
