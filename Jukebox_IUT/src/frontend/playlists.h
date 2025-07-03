#ifndef PLAYLISTS_H
#define PLAYLISTS_H

#include <QWidget>


QT_BEGIN_NAMESPACE
namespace Ui { class playlists; }
QT_END_NAMESPACE

class playlists : public QWidget {
Q_OBJECT

public:
    explicit playlists(QWidget *parent = nullptr);
    ~playlists() override;

public slots:
    void onAddButtonClicked();
    void onPlaylistsChanged();

private:
    Ui::playlists *ui;

    void renderPlaylists();
};


#endif //PLAYLISTS_H
