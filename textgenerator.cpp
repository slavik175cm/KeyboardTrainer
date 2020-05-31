#include "textgenerator.h"
#include <qfile.h>
TextGenerator::TextGenerator() {
    srand(time(0));
    read_frequency_table();
}

void TextGenerator::read_frequency_table() {
    QFile file("D:\\qt_projects\\KeyboardTrainer\\FrequencyTable.txt");
    file.open(QIODevice::ReadWrite);
    for (int i = 0; i < 26; i++) {
        QStringList line = QString(file.readLine()).split(' ');
        for (int j = 0; j < 26; j++) {
            frequency_table[i][j] = line[j].toInt();
            if (frequency_table[i][j] < 20) frequency_table[i][j] = 0;
        }
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
        int word_len = rand() % 6 + 3;
        if (len  <= text.size() + word_len + 3)
            word_len = len - text.size();

        QString word = QString(first_letter());
        bool ok = 1;
        for (int i = 1; i < word_len; i++) {
            word += next_letter(word[i - 1]);
            //checking for three consonant or vowel symbols in a row
            if (i >= 2) {
                bool all_vowels = 1, all_consonants = 1;

                for (int j = i - 2; j <= i; j++) {
                    all_vowels &= vowels.contains(word[j].toUpper().unicode());
                    all_consonants &= consonants.contains(word[j].toUpper().unicode());
                }

                if (all_vowels || all_consonants) {
                    ok = 0;
                    break;
                }
            }
        }

        if (!ok) continue;
        text += word;
        if (text.size() < len)
            text += ' ';
    }
    return text;
}
