#include "user.h"
#include <qDebug>

User::User(DataBase *db)
{
    this->db = db;
    name = nullptr;
    letter_pressed.resize(26);
    letter_mistakes.resize(26);
    for (int i = 0; i < 26; i++) {
        letter_mistakes[i] = 0;
        letter_pressed[i] = 0;
    }
    speed.resize(0);
}

void User::add_new_sample(int current_speed, QVector<int> letter_pressed, QVector<int> letter_mistakes) {
    speed.push_back(current_speed);
    for (int i = 0; i < 26; i++) {
        this->letter_pressed[i] += letter_pressed[i];
        this->letter_mistakes[i] += letter_mistakes[i];
    }
    if (name != nullptr)
        db->update(this);
}
