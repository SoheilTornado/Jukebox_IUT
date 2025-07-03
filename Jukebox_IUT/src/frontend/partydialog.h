#ifndef PARTYDIALOG_H
#define PARTYDIALOG_H

#include <QDialog>

namespace Ui {
class partydialog;
}

class partydialog : public QDialog
{
    Q_OBJECT

public:
    explicit partydialog(QWidget *parent = nullptr);
    ~partydialog();

    enum PartyChoice {
        noChoice= QDialog::Accepted + 2,
        createParty,
        joinParty,
        disconnectParty
    };
private slots:
    void on_btnCreateServer_clicked();

    void on_btnJoinServer_clicked();

private:
    Ui::partydialog *ui;
};

#endif // PARTYDIALOG_H
