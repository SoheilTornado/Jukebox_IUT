#ifndef USERMANAGER_H
#define USERMANAGER_H

#include <QString>
#include <QVector>

struct User {
    QString name;
    QString username;
    QString mail;
    QString passwordHash;
    QString hint;
};

class UserManager {
public:
    static QVector<User> users;

    static void loadFromFile();
    static void saveToFile();

    static bool usernameExists(const QString& username);
    static QString getHint(const QString& username);
    static void addUser(const User& user);
    static bool validateLogin(const QString& username, const QString& passwordHash);

    static QString hashPassword(const QString& password);

private:
    static QString encrypt(const QString& data);
    static QString decrypt(const QString& data);
    static QByteArray getKey();
};

#endif // USERMANAGER_H
