#ifndef CREATESERVERDIALOG_H
#define CREATESERVERDIALOG_H

#include <QDialog>

class NetworkManager;
namespace Ui { class createserverdialog; }

class createserverdialog : public QDialog
{
    Q_OBJECT

public:
    explicit createserverdialog(NetworkManager *networkManager, QWidget *parent = nullptr);
    ~createserverdialog();

private slots:
    void on_btnCloseParty_clicked();
    void updateClientList();
    void onClientIdentified(const QString& username);

private:
    Ui::createserverdialog *ui;
    NetworkManager *m_networkManager;

    void setupServerInfo();
    QString getLocalIpAddress();
};

#endif // CREATESERVERDIALOG_H
