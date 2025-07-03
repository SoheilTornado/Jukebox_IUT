#include "queue.h"

#include "queuesong.h"
#include "ui_queue.h"
#include "../backend/queueManager.h"


queue::queue(QWidget *parent) :
    QWidget(parent), ui(new Ui::queue) {
    ui->setupUi(this);

    connect(queueManager::getInstance(), &queueManager::queueChanged, this, &queue::onQueueChanged);
    connect(ui->clearButton, &QPushButton::clicked, queueManager::getInstance(), &queueManager::onClearQueue);
}

//calls renderSongs function when queue changed
void queue::onQueueChanged() {
    renderSongs();
}

//renders all song widgets in the queue window
void queue::renderSongs() {
    ui->listWidget->clear();

    const auto queue = queueManager::getInstance()->getQueue();
    for (int i = 0; i < queue.size(); i++) {
        const auto item = new QListWidgetItem(ui->listWidget);
        const auto songWidget = new queueSong(this, queue.at(i), i);

        if (i == 0) {
            songWidget->setStyleSheet("#songContent { background-color: #1A574B; }");

            const auto textItem = new QListWidgetItem(ui->listWidget);
            const auto label = new QLabel(this);
            label->setText("Up Next");
            textItem->setSizeHint(QSize(0, 30));
            ui->listWidget->addItem(textItem);
            ui->listWidget->setItemWidget(textItem, label);
        }

        item->setSizeHint(songWidget->sizeHint());

        ui->listWidget->addItem(item);
        ui->listWidget->setItemWidget(item, songWidget);
    }
}

queue::~queue() {
    delete ui;
}
