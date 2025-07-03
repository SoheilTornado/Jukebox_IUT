#ifndef QUEUESONG_H
#define QUEUESONG_H

#include <QWidget>

#include "../backend/song.h"


QT_BEGIN_NAMESPACE
namespace Ui { class queueSong; }
QT_END_NAMESPACE

class queueSong : public QWidget {
Q_OBJECT

public:
    explicit queueSong(QWidget *parent = nullptr, const song &s = song(QUrl(), QPixmap(), "Unknown", "Unknown", "00:00"), int index = 0);
    ~queueSong() override;

private slots:
    void onRemoveFromQueueClicked();

signals:
    void removeFromQueue(int entryNum) const;

private:
    Ui::queueSong *ui;
    song thisSong;
    int index;
};


#endif //QUEUESONG_H
