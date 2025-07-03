#ifndef PLAYLIST_H
#define PLAYLIST_H

#include "song.h"
#include <QVector>

class playlist : public QVector<song>{
public:
    explicit playlist(QString name, QUrl coverUrl = QUrl()) : name(std::move(name)), coverUrl(std::move(coverUrl)) { id = QUuid::createUuid(); }

    void appendRef(const song &s) { this->append(s); }

    [[nodiscard]] QUuid getId() const { return id; }
    [[nodiscard]] QString getName() const { return name; }
    [[nodiscard]] QUrl getCoverUrl() const { return coverUrl; } // متد جدید

    void setName(const QString &newName) { name = newName; }

private:
    QUuid id;
    QString name;
    QUrl coverUrl; // متغیر جدید برای ذخیره آدرس کاور
};

#endif //PLAYLIST_H
