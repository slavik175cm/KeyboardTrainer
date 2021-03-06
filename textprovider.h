#ifndef TEXTPROVIDER_H
#define TEXTPROVIDER_H
#include <QString>
#include <QKeyEvent>
#include <QTextBrowser>
#include <statisticsprovider.h>
#include "user.h"
#include "textgenerator.h"
#include "QLCDNumber"
class TextProvider : QObject
{
public:
    QTextBrowser *text_field;
    QTextBrowser *symbols_per_minute;
    QTextBrowser *number_of_errors;

    int current = 0, current_block = 0, text_length = 0;
    int wrong_symbols = 0;
    QString text, block;
    int block_len, blocksize, block_rows, last_not_taken = 0;
    qint64 time_started = 0;
    bool cursor_visible = 1, random_mode = 1;
    QStringList list_of_words;
    QFont *myFont;
    QFontMetrics *fm;
    const QVector<int> available_symbols = {'.', ',', ';', '(', ')', ' ', '"', '\'', ':', '-', '+', '!', '@', '#', '$', '%', '^', '*', '|'};
    QVector<int> letter_pressed_count;
    QVector<int> letter_mistakes;
    TextGenerator *text_generator;
    User *user;


    TextProvider(QTextBrowser *text_field, QTextBrowser *symbols_per_minute, QTextBrowser *number_of_errors, TextGenerator *text_generator, User *user);
    void change_font(QFont *new_font);
    void change_font_size(int new_size);
    void next_block();
    void add_html(QString &text);
    void input_letter(QKeyEvent *event);
    void change_letter_color(int pos, QString color);
    void change_letter_background(int pos, QString color);
    void restart();
    void restart(QString new_text);
    void timerEvent(QTimerEvent *event) override;
};

#endif // TEXTPROVIDER_H
