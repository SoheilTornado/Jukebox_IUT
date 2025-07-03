#ifndef SWITCHER_H
#define SWITCHER_H

#include <QWidget>

#include "editor.h"
#include "library.h"
#include "../backend/playlist.h"


QT_BEGIN_NAMESPACE
namespace Ui { class switcher; }
QT_END_NAMESPACE

class switcher final : public QWidget {
Q_OBJECT

public:
    static switcher* getInstance();
    ~switcher() override;

public slots:
    void onEditPlaylist(int playlistIndex);
    void onBackButtonClicked();

private:
    static switcher* instance;
    Ui::switcher *ui;
    library* lib;
    editor* edit{};
    QString currentPlaylist;

    explicit switcher(QWidget *parent = nullptr);
};


#endif //SWITCHER_H
