#ifndef USER_H
#define USER_H

#include <QString>
#include <QVector>
#include "database.h"
class DataBase;
class User
{
public:
    User(DataBase *db);
    QString name, password;
    QVector<int> letter_pressed_count;
    QVector<int> letter_mistakes;
    QVector<int> speed;
    void add_new_sample(int current_speed, QVector<int> letter_pressed_count, QVector<int> letter_mistakes);
    DataBase *db;

};

#endif // USER_H
