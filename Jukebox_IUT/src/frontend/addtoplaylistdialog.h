#ifndef ADDTOPLAYLISTDIALOG_H
#define ADDTOPLAYLISTDIALOG_H

#include <qdialog.h>
#include <QWidget>

#include "librarysong.h"
#include "../backend/song.h"


QT_BEGIN_NAMESPACE
namespace Ui { class addtoplaylistdialog; }
QT_END_NAMESPACE

class addtoplaylistdialog final : public QDialog {
Q_OBJECT

public:
    explicit addtoplaylistdialog(QWidget *parent = nullptr, const song &s = song(QUrl(), QPixmap(), "Unknown", "Unknown", "00:00"));
    ~addtoplaylistdialog() override;

private:
    Ui::addtoplaylistdialog *ui;
};


#endif //ADDTOPLAYLISTDIALOG_H
