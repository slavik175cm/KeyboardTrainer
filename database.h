#ifndef DATABASE_H
#define DATABASE_H
#include <QtSql>
#include <QtDebug>
#include "user.h"
class User;
class DataBase
{
public:
    DataBase();
    User* get_user(QString name);
    void add_user(User *user);
    void update(User *user);
    bool is_name_exist(QString name);
    bool is_password_correct(QString name, QString password);
private:
    QSqlDatabase db;
    QSqlQuery *query;
    QString qry;
};

#endif // DATABASE_H
