#ifndef JOINPARTYDIALOG_H
#define JOINPARTYDIALOG_H

#include <QDialog>

class NetworkManager;
namespace Ui { class joinpartydialog; }

class joinpartydialog : public QDialog
{
    Q_OBJECT

public:
    explicit joinpartydialog(NetworkManager *networkManager, QWidget *parent = nullptr);
    ~joinpartydialog();

private slots:
    void on_btnConnectToServer_clicked();
    void on_btnDisconnectFromServer_clicked();
    void onConnectedSuccessfully();
    void onConnectionError(const QString &error);
    void onHostIdentified(const QString &username);

private:
    Ui::joinpartydialog *ui;
    NetworkManager *m_networkManager;
};

#endif // JOINPARTYDIALOG_H
