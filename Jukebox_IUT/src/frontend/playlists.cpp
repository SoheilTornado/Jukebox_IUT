#include "playlists.h"
#include "createplaylistdialog.h"
#include "playlistinlist.h"
#include "ui_playlists.h"
#include "../backend/playlistManager.h"


playlists::playlists(QWidget *parent) :
    QWidget(parent), ui(new Ui::playlists) {
    ui->setupUi(this);

    renderPlaylists();

    connect(ui->addButton, &QPushButton::clicked, this, &playlists::onAddButtonClicked);
    connect(playlistManager::getInstance(), &playlistManager::playlistsChanged, this, &playlists::onPlaylistsChanged);
}

//calls renderPlaylists function when playlists changed
void playlists::onPlaylistsChanged() {
    renderPlaylists();
}

//renders all playlist widgets in the playlist window
void playlists::renderPlaylists() {
    ui->playlistList->clear();

    int index = 0;
    for (const auto& playlist: playlistManager::getInstance()->getPlaylists()) {
        const auto item = new QListWidgetItem(ui->playlistList);
        const auto playlistWidget = new playlistinlist(this, playlist, index);

        item->setSizeHint(playlistWidget->sizeHint());

        ui->playlistList->addItem(item);
        ui->playlistList->setItemWidget(item, playlistWidget);
        index++;
    }
}

//opens a dialog window to create a new playlist
void playlists::onAddButtonClicked() {
    const auto createDialog = new createplaylistdialog(this);

    createDialog->exec();
}

playlists::~playlists() {
    delete ui;
}
