#include "textprovider.h"
#include <QTimerEvent>
#include <QKeyEvent>
#include <QDateTime>
#include <QTimer>
#include <QDebug>

TextProvider::TextProvider(QTextBrowser *text_field, QTextBrowser *symbols_per_minute, QTextBrowser *number_of_errors, TextGenerator *text_generator, User *user)
{
    this->text_field = text_field;
    this->symbols_per_minute = symbols_per_minute;
    this->number_of_errors = number_of_errors;
    this->text_generator = text_generator;
    this->user = user;
    myFont = new QFont(text_field->font().family(), text_field->font().pointSize());
    fm = new QFontMetrics(*myFont);

    letter_pressed_count.resize(26);
    letter_mistakes.resize(26);

    startTimer(300);
}

void TextProvider::change_font(QFont *new_font) {
    myFont = new QFont(new_font->family(), myFont->pointSize());
    fm = new QFontMetrics(*myFont);
    text_field->setFont(*myFont);
    last_not_taken = 0;
    block_rows = (text_field->height() / fm->boundingRect(text).height());
    restart();
}

void TextProvider::change_font_size(int new_size) {
    myFont = new QFont(myFont->family(), new_size);
    fm = new QFontMetrics(*myFont);
    text_field->setFont(*myFont);
    last_not_taken = 0;
    block_rows = (text_field->height() / fm->boundingRect(text).height());
    restart();
}

void TextProvider::next_block() {
    current = 0;
    int taken_rows = 0;
    block = "";
    QString cur = "";
    for (int i = last_not_taken; i < list_of_words.size(); i++) {
        cur += list_of_words[i] + " ";
        if (i == list_of_words.size() - 1 || (fm->horizontalAdvance(cur + list_of_words[i + 1]) > text_field->width() - 10)) {
            last_not_taken = i + 1;
            taken_rows++;
            block += cur;
            cur = "";
            if (i == list_of_words.size() - 1 || taken_rows == block_rows) {
                block.remove(block.size() - 1, 1);
                block_len = block.size();
                text = block;
                add_html(block);
                text_field->setText(block);
                return;
            }
        }
    }
}

void TextProvider::add_html(QString &text) {
    QString new_text = "";
    for (int i = 0; i < text.size(); i++) {
        new_text += "<font color=#9CA7BE>";
        new_text += "<span style=\"background-color: #4D535E\">";
        new_text += text[i];
    }
    text = new_text;
}

void TextProvider::input_letter(QKeyEvent *event) {
    static bool wrong_letter = 0;
    if (event->key() == Qt::Key_Escape) {
        restart();
        wrong_letter = 0;
        return;
    }
    int c = event->key();
    if ((tolower(c) < 'a' || tolower(c) > 'z') && (c < '0' || c > '9') && !available_symbols.contains(c))
        return;
    qDebug() << event->key();
    if (c >= 'A' && c <= 'Z' && event->modifiers() != Qt::ShiftModifier)
        c = tolower(c);

    if (time_started == 0)
        time_started = QDateTime::currentMSecsSinceEpoch();
    if (text[current] == c) {
        change_letter_background(current, "#4D535E");
        change_letter_color(current, wrong_letter ?  "#FA4940" : "#BBA8AD");
        wrong_letter = 0;
        current++;
        if (tolower(c) >= 'a' && tolower(c) <= 'z')
            letter_pressed_count[tolower(c) - 'a']++;

        if (current == block_len && last_not_taken == list_of_words.size()) {
            int spm = (qint64)text_length * 1000 * 60 / (QDateTime::currentMSecsSinceEpoch() - time_started);
            symbols_per_minute->setText("<p align=\"center\"><font color=#BBA8AD>" + QString::number(spm));
            number_of_errors->setText("<p align=\"center\"><font color=#FF4500>" + QString::number(wrong_symbols));
            if (random_mode == 1) {
                user->add_new_sample(spm, letter_pressed_count, letter_mistakes);
                restart(text_generator->generate_random_text(text_length));
            } else restart();
            return;
        }
        if (current == block_len) {
            next_block();
            return;
        }
        change_letter_background(current, "#48444A");
    } else {
        wrong_letter = 1;
        if (text[current].toLower() >= 'a' && text[current].toLower() <= 'z') {
            letter_mistakes[text[current].toLower().unicode() - 'a']++;
            letter_pressed_count[text[current].toLower().unicode() - 'a']++;
        }
        wrong_symbols++;
    }
}

void TextProvider::change_letter_color(int pos, QString color) {
    int realpos = pos  * 61 + 12;
    block.remove(realpos, 7);
    block.insert(realpos, color);
    text_field->setText(block);
}

void TextProvider::change_letter_background(int pos, QString color) {
    int realpos = pos  * 61 + 20 + 31;
    block.remove(realpos, 7);
    block.insert(realpos, color);
    text_field->setText(block);
}

void TextProvider::restart() {
    last_not_taken = 0;
    time_started = 0;
    cursor_visible = 1;
    wrong_symbols = 0;
    for (int i = 0; i < 26; i++) {
        letter_mistakes[i] = 0;
        letter_pressed_count[i] = 0;
    }
    next_block();
}

void TextProvider::restart(QString new_text) {
    text = new_text;
    text_length = text.length();
    list_of_words = text.split(' ');
    block_rows = (text_field->height() / fm->boundingRect(text).height());
    restart();
}


void TextProvider::timerEvent(QTimerEvent *event) {
    change_letter_background(current, cursor_visible ? "#48444A" : "#4D535E");
    cursor_visible ^= 1;
}

