#ifndef PLAYLISTMANAGER_H
#define PLAYLISTMANAGER_H
#include <QObject>
#include <QString>
#include "playlist.h"
#include "song.h"

class playlistManager final : public QObject {
    Q_OBJECT
public:
    static playlistManager* getInstance();

    [[nodiscard]] QVector<playlist> getPlaylists() const { return playlists; }

    void addTrackToPlaylist(const QString& playlistName, const song& s);

    void upsertPlaylist(const playlist& playlist);

    void createPlaylist(const QString& name, QUrl url);

    void loadPlaylists();

    void deletePlaylist(int index);

    void setUsername(const QString &username);

signals:
    void playlistsChanged();

public slots:
    void onDeletePlaylist(int index);

    void onDeleteSongFromPlaylist(int playlistIndex, int songIndex);


private slots:
    void onLibraryChanged();

private:
    static playlistManager *instance;
    QVector<playlist> playlists;

    playlistManager();
    song fromSaveString(const QString& saveString);
    QString m_username;

};

#endif //PLAYLISTMANAGER_H
