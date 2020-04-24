#include "textgenerator.h"
#include <qfile.h>

TextGenerator::TextGenerator() {
    read_frequency_table();
}

void TextGenerator::read_frequency_table() {
    QFile file("D:\\qt_projects\\KeyboardTrainer\\FrequencyTable.txt");
    file.open(QIODevice::ReadWrite);
    for (int i = 0; i < 26; i++) {
        QStringList line = QString(file.readLine()).split(' ');
        for (int j = 0; j < 26; j++)
            frequency_table[i][j] = line[j].toInt();
    }
}

char TextGenerator::next_letter(QChar ch) {
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

char TextGenerator::first_letter() {
    int sum = 0;
    for (int i = 0; i < 26; i++)
        sum += first_letter_frequency[i];
    int rnd = rand() % sum;
    sum = 0;
    for (int i = 0; i < 26; i++) {
        sum += first_letter_frequency[i];
        if (sum >= rnd) return 'a' + i;
    }
}

QString TextGenerator::generate_random_text(int len) {
    QString text = "";
    while (text.size() < len) {
        int word_len = qMin(rand() % 6 + 1, len - text.size());
        QString word = QString(first_letter());
        bool ok = 0;
        for (int i = 0; i < word_len; i++) {
            QChar next = next_letter(word[i]);
            while (i >= 1 && consonant.contains(word[i].toUpper().unicode()) &&
                   consonant.contains(word[i - 1].toUpper().unicode()) && consonant.contains(next.toUpper().unicode()))
                next = next_letter(word[i]);
            if (vowels.contains(next.toUpper().unicode())) ok = 1;
            word += next;
        }
        if (!ok) continue;
        text += word;
        if (text.size() < len) text += ' ';
    }
    return text;
}
