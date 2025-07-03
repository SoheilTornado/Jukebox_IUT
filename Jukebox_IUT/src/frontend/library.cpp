#include "library.h"
#include "librarysong.h"

#include <QMediaMetaData>

#include <QMenu>
#include "ui_library.h"
#include "../backend/libraryManager.h"
#include "../backend/playlistManager.h"


library::library(QWidget *parent) :
    QWidget(parent), ui(new Ui::library), searchResults(QVector<song>()) {
    ui->setupUi(this);

    renderSongs();

    connect(libraryManager::getInstance(), &libraryManager::libraryChanged, this, &library::onLibraryChanged);
    connect(ui->addButton, &QPushButton::clicked, this, &library::onAddButtonClicked);
    connect(ui->searchInput, &QLineEdit::textEdited, this, &library::onSearch);
    connect(ui->refresh, &QPushButton::clicked, libraryManager::getInstance(), &libraryManager::onReloadLibrary);
    connect(libraryManager::getInstance(), &libraryManager::libraryChanged, this, &library::onSearchInitiated);

}

//calls renderSongs function when library changed
void library::onLibraryChanged() {
    renderSongs();
}

//renders all song widgets in the library window
void library::renderSongs() {
    ui->songList->clear();

    if (noResults)
    {
        const auto textItem = new QListWidgetItem(ui->songList);
        const auto label = new QLabel(this);
        label->setText("No songs or artists match your search.");
        textItem->setSizeHint(QSize(0, 30));
        ui->songList->addItem(textItem);
        ui->songList->setItemWidget(textItem, label);

        return;
    }

    const auto toRender = !searchResults.isEmpty() ? searchResults : libraryManager::getInstance()->getLibrary();
    for (const song &song: toRender) {
        const auto item = new QListWidgetItem(ui->songList);
        const auto songWidget = new librarysong(this, song);

        item->setSizeHint(songWidget->sizeHint());

        ui->songList->addItem(item);
        ui->songList->setItemWidget(item, songWidget);
    }
}

//opens a file dialog when the addButton is clicked to add a directory to the library
void library::onAddButtonClicked() const
{
    libraryManager::getInstance()->addDirectory();
}


void library::onSearchInitiated() {
    const auto searchTerm = ui->searchInput->text();
    onSearch(searchTerm);
}

void library::onSearch(const QString &searchTerm) {
    noResults = false;
    searchResults.clear();

    if (searchTerm.startsWith("https://") || searchTerm.startsWith("http://") || searchTerm.isEmpty() || searchTerm.isNull() || searchTerm == QString()) {
        renderSongs();
        return;
    }

    const auto library = libraryManager::getInstance()->getLibrary();
    const QStringList searchTerms = searchTerm.split(" ", Qt::SkipEmptyParts);

    for (const song &song: library) {
        bool found = true;
        for (const QString &term: searchTerms) {
            if (!song.getTitle().contains(term, Qt::CaseInsensitive) &&
                !song.getArtist().contains(term, Qt::CaseInsensitive)) {
                found = false;
                break;
            }
        }
        if (found) {
            searchResults.append(song);
        }
    }
    noResults = searchResults.isEmpty();
    renderSongs();
}

library::~library() {
    delete ui;
}
