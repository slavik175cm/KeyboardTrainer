#ifndef TEXTGENERATOR_H
#define TEXTGENERATOR_H
#include <QString>
#include <QVector>
 class TextGenerator
{
public:
    QString generate_random_text(int len);
    TextGenerator();
private:
    void read_frequency_table();
    char first_letter() ;
    char next_letter(QChar ch);
    const int first_letter_frequency[26] = {11682, 4434, 5238, 3174, 2799, 4027, 1642, 4200, 7294, 511, 856, 2415, 3826, 2284, 7631, 4319, 222, 2826, 6686, 15978, 1183, 824, 5497, 45, 763, 45}; // a..z
    int frequency_table[26][26];
    const QVector<char> consonant = {'B', 'C', 'D', 'F', 'G', 'H', 'J', 'K', 'L', 'M', 'N', 'P', 'Q', 'R', 'S', 'T', 'V', 'X', 'W', 'Z'};
    const QVector<char> vowels = {'A', 'E', 'I', 'O', 'U', 'Y'};
};

#endif // TEXTGENERATOR_H
