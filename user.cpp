#include "user.h"
#include <qDebug>

User::User(DataBase *db)
{
    this->db = db;
    name = nullptr;
    letter_pressed_count.resize(26);
    letter_mistakes.resize(26);
    for (int i = 0; i < 26; i++) {
        letter_mistakes[i] = 0;
        letter_pressed_count[i] = 0;
    }
    speed.resize(0);
}

void User::add_new_sample(int current_speed, QVector<int> letter_pressed_count, QVector<int> letter_mistakes) {
    speed.push_back(current_speed);
    for (int i = 0; i < 26; i++) {
        this->letter_pressed_count[i] += letter_pressed_count[i];
        this->letter_mistakes[i] += letter_mistakes[i];
    }

    if (name != nullptr)
        db->update(this);
}
