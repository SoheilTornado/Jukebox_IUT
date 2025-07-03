#include "UserManager.h"
#include <QFile>
#include <QTextStream>
#include <QCryptographicHash>
#include <QByteArray>

QVector<User> UserManager::users;

void UserManager::loadFromFile() {
    users.clear();
    QFile file("users.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return;

    QTextStream in(&file);
    while (!in.atEnd()) {
        User user;
        user.username = decrypt(in.readLine());
        user.name = decrypt(in.readLine());
        user.mail = decrypt(in.readLine());
        user.passwordHash = in.readLine();
        user.hint = decrypt(in.readLine());
        QString separator = in.readLine(); // "---"

        if (!user.username.isEmpty())
            users.append(user);
    }

    file.close();
}

void UserManager::saveToFile() {
    QFile file("users.txt");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) return;

    QTextStream out(&file);
    for (const auto& user : users) {
        out << encrypt(user.username) << "\n"
            << encrypt(user.name) << "\n"
            << encrypt(user.mail) << "\n"
            << user.passwordHash << "\n"
            << encrypt(user.hint) << "\n"
            << "---\n";
    }

    file.close();
}

bool UserManager::usernameExists(const QString& username) {
    for (const auto& user : users)
        if (user.username == username)
            return true;
    return false;
}

void UserManager::addUser(const User& user) {
    users.append(user);
    saveToFile();
}

bool UserManager::validateLogin(const QString& username, const QString& passwordHash) {
    for (const auto& user : users)
        if (user.username == username && user.passwordHash == passwordHash)
            return true;
    return false;
}

QString UserManager::getHint(const QString& username) {
    for (const auto& user : users)
        if (user.username == username)
            return user.hint;
    return "";
}

QString UserManager::hashPassword(const QString& password) {
    QByteArray hash = QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256);
    return QString(hash.toHex());
}

// ------------------ Encryption Helpers ------------------

QByteArray UserManager::getKey() {
    QByteArray key = QCryptographicHash::hash("some-secret-key", QCryptographicHash::Sha256);
    return key.left(16); // AES key size = 16 bytes
}

QString UserManager::encrypt(const QString& data) {
    QByteArray raw = data.toUtf8();
    QByteArray key = getKey();
    QByteArray result;

    for (int i = 0; i < raw.size(); ++i)
        result.append(raw[i] ^ key[i % key.size()]);

    return QString(result.toBase64());
}

QString UserManager::decrypt(const QString& data) {
    QByteArray raw = QByteArray::fromBase64(data.toUtf8());
    QByteArray key = getKey();
    QByteArray result;

    for (int i = 0; i < raw.size(); ++i)
        result.append(raw[i] ^ key[i % key.size()]);

    return QString::fromUtf8(result);
}
