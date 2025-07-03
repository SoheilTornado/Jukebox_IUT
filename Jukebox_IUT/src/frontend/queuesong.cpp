#include "queuesong.h"
#include "ui_queuesong.h"
#include "../backend/queueManager.h"


queueSong::queueSong(QWidget *parent, const song &s, const int index) :
    QWidget(parent), ui(new Ui::queueSong), thisSong(s), index(index) {
    ui->setupUi(this);

    ui->cover->setPixmap(s.getCover());
    ui->title->setText(s.getTitle());
    ui->artist->setText(s.getArtist());

    if (index == 0) {
        ui->removeFromQueue->setStyleSheet("#removeFromQueue:hover {background-color: #267B6A;}");
    }

    connect(ui->removeFromQueue, &QPushButton::clicked, this, &queueSong::onRemoveFromQueueClicked);
    connect(this, &queueSong::removeFromQueue, queueManager::getInstance(), &queueManager::onRemoveFromQueue);
}

//emits removeFromQueue with the index when the remove button is clicked in the queue window for a song
void queueSong::onRemoveFromQueueClicked() {
    emit removeFromQueue(index);
}

queueSong::~queueSong() {
    delete ui;
}
