#include "playlistManager.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QDir>
#include "libraryManager.h"

playlistManager *playlistManager::instance = nullptr;

playlistManager::playlistManager() : playlists(QVector<playlist>()) {
    connect(libraryManager::getInstance(), &libraryManager::libraryChanged, this, &playlistManager::onLibraryChanged);
}

playlistManager* playlistManager::getInstance()
{
    if (instance == nullptr) {
        instance = new playlistManager();
    }
    return instance;
}

void playlistManager::setUsername(const QString &username)
{
    m_username = username;
    loadPlaylists();
}

void playlistManager::onLibraryChanged() {
    loadPlaylists();
}

void playlistManager::upsertPlaylist(const playlist& playlist) {
    if (m_username.isEmpty()) {
        qWarning() << "Cannot save playlist, username is not set!";
        return;
    }

    QString filename = m_username + "-" + playlist.getName() + ".txt";
    QDir dir("./playlists");
    if (!dir.exists() && !dir.mkpath(".")) {
        qWarning() << "Failed to create playlists directory";
        return;
    }

    QFile file("./playlists/" + filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
        qWarning() << "Failed to open " << filename;
        return;
    }

    QTextStream stream(&file);
    for (const auto& song: playlist) {
        stream << song.toSaveString() << "\n";
    }
    file.close();
}

void playlistManager::createPlaylist(const QString& name, QUrl url) {
    for (const auto& currPlaylist: playlists) {
        if (currPlaylist.getName() == name) {
            qWarning() << "Playlist with name " << name << " already exists";
            return;
        }
    }
    const auto newPlaylist = playlist(name, url);
    upsertPlaylist(newPlaylist);

    playlists.append(newPlaylist);
    emit playlistsChanged();
}
void playlistManager::loadPlaylists() {
    playlists.clear();

    if (m_username.isEmpty()) {
        qWarning() << "Cannot load playlists, username is not set!";
        emit playlistsChanged();
        return;
    }

    QDir dir("./playlists");
    if (!dir.exists()) {
        return;
    }

    QString userPrefix = m_username + "-";
    for (const auto& file : dir.entryList(QStringList() << userPrefix + "*.txt", QDir::Files)) {
        QFile playlistFile("./playlists/" + file);
        if (!playlistFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
            qWarning() << "Failed to open " << file;
            continue;
        }

        QString cleanName = file;
        cleanName.remove(userPrefix);
        cleanName.chop(4);

        playlist newPlaylist(cleanName);
        QTextStream stream(&playlistFile);
        while (!stream.atEnd()) {
            const auto line = stream.readLine();
            auto song = fromSaveString(line);
            newPlaylist.append(song);
        }

        playlists.append(newPlaylist);
    }
    emit playlistsChanged();
}
song playlistManager::fromSaveString(const QString& saveString) {
    for (auto& s: libraryManager::getInstance()->getLibrary()) {
        if (s.getUrl() == QUrl(saveString)) {
            return s;
        }
    }
    return song(QUrl(), QPixmap(":image/placeholder.png"), "Not Found", "Not Found", "Not Found");
}

void playlistManager::addTrackToPlaylist(const QString& playlistName, const song& s) {
    for (auto& p: playlists) {
        if (p.getName() == playlistName) {
            p.append(s);
            upsertPlaylist(p);
            emit playlistsChanged();
            return;
        }
    }
}

void playlistManager::deletePlaylist(const int index) {
    if (index < 0 || index >= playlists.size()) return;

    QString filename = m_username + "-" + playlists[index].getName() + ".txt";
    QFile file("./playlists/" + filename);
    if (!file.remove()) {
        qWarning() << "Failed to delete " << filename;
    }
    playlists.remove(index);
    emit playlistsChanged();
}

void playlistManager::onDeleteSongFromPlaylist(const int playlistIndex, const int songIndex) {
    if (playlistIndex < 0 || playlistIndex >= playlists.size()) return;
    playlists[playlistIndex].remove(songIndex);
    upsertPlaylist(playlists[playlistIndex]);
    emit playlistsChanged();
}

void playlistManager::onDeletePlaylist(const int index) {
    deletePlaylist(index);
}
