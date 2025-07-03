#include "createplaylistdialog.h"

#include <QFileDialog>

#include "ui_createplaylistdialog.h"
#include "../backend/playlistManager.h"


createplaylistdialog::createplaylistdialog(QWidget *parent) :
    QDialog(parent), ui(new Ui::createplaylistdialog) {
    ui->setupUi(this);

    connect(ui->create, &QPushButton::clicked, this, &createplaylistdialog::onCreateClicked);
    connect(ui->chooseCover, &QPushButton::clicked, this, &createplaylistdialog::onChooseCoverClicked);
}

//creates a new playlist when the create button in createPlaylist dialog is clicked and closes the dialog
void createplaylistdialog::onCreateClicked() {
    const auto playlistName = ui->playlistName->text();
    if (playlistName.isEmpty()) {
        qWarning() << "Playlist Name cant be empty";
        return;
    }

    const QUrl coverUrl = QUrl::fromLocalFile(ui->coverUrl->text());

    playlistManager::getInstance()->createPlaylist(playlistName, coverUrl);

    close();
}
//opens a file dialog to choose a cover for the playlist
void createplaylistdialog::onChooseCoverClicked() {
    const auto coverPath = QFileDialog::getOpenFileName(this, "Choose cover", QDir::homePath(), "Images (*.png *.jpg)");
    ui->coverUrl->setText(coverPath);
}

createplaylistdialog::~createplaylistdialog() {
    delete ui;
}
