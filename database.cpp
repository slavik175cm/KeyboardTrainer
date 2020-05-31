#include "database.h"

DataBase::DataBase()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("UsersDataBase.db");
    db.open();
    query = new QSqlQuery(db);
}

User* DataBase::get_user(QString name) {
    query->exec("SELECT * FROM users WHERE name='" + name + "';");
    query->next();
    QSqlRecord rec = query->record();
    User *user = new User(this);
    user->name = query->value(rec.indexOf("name")).toString();
    user->password = query->value(rec.indexOf("password")).toString();
    for (int i = 0; i < 26; i++) {
        user->letter_pressed[i] = query->value(rec.indexOf(QString('A' + i) + "_pressed")).toInt();
        user->letter_mistakes[i] = query->value(rec.indexOf(QString('A' + i) + "_mistakes")).toInt();
    }

    query->exec("SELECT * FROM " + user->name + "_samples;");
    rec = query->record();
    while (query->next())
        user->speed.push_back(query->value(rec.indexOf("speed")).toInt());
    return user;
}

void DataBase::add_user(User *user) {
    if (is_name_exist(user->name)) return;
    qry = "INSERT INTO users (name, password";
    for (int i = 0; i < 26; i++) {
        qry += ", " + QString('A' + i) + "_pressed";
        qry += ", " + QString('A' + i) + "_mistakes";
    }
    qry += ") VALUES ('" + user->name + "', '" + user->password + '\'';
    for (int i = 0; i < 26; i++) {
        qry += ", " + QString::number(user->letter_pressed[i]);
        qry += ", " + QString::number(user->letter_mistakes[i]);
    }
    qry += ");";
    query->exec(qry);

    query->exec("CREATE TABLE " + user->name + "_samples (id INTEGER PRIMARY KEY AUTOINCREMENT, speed INT);");
    for (int i = 0; i < user->speed.size(); i++)
        query->exec("INSERT INTO " + user->name + "_samples (speed) VALUES (" + QString::number(user->speed[i]) + ");");
}

void DataBase::update(User *user) {
    qry = "UPDATE users SET ";
    for (int i = 0; i < 26; i++) {
        if (i != 0) qry += ", ";
        qry += QString('A' + i) + "_pressed = '" + QString::number(user->letter_pressed[i]) + '\'';
        qry += ", " + QString('A' + i) + "_mistakes = '" + QString::number(user->letter_mistakes[i]) + '\'';
    }
    qry += " WHERE name = '" + user->name + "';";
    query->exec(qry);

    query->exec("SELECT COUNT(*) FROM " + user->name + "_samples;");
    query->next();
    int table_size = query->value(0).toInt();
    for (int i = table_size; i < user->speed.size(); i++)
        query->exec("INSERT INTO " + user->name + "_samples (speed) VALUES (" + QString::number(user->speed[i]) + ");");
}

bool DataBase::is_name_exist(QString name) {
    query->exec("SELECT COUNT(*) FROM users WHERE name='" + name +"';");
    query->next();
    return query->value(0).toInt() > 0;
}

bool DataBase::is_password_correct(QString name, QString password) {
    if (!is_name_exist(name)) return false;
    query->exec("SELECT password FROM users WHERE name='" + name +"';");
    if (!query->next())
        return false;
    return query->value(0) == password;
}
