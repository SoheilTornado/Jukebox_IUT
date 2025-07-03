#include "joinpartydialog.h"
#include "ui_joinpartydialog.h"
#include "../backend/networkmanager.h"
#include <QHostAddress>

joinpartydialog::joinpartydialog(NetworkManager *networkManager, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::joinpartydialog)
    , m_networkManager(networkManager)
{
    ui->setupUi(this);
    setWindowTitle("Join Party");


    ui->lblServerName->clear();

    connect(m_networkManager, &NetworkManager::connectedToPeer, this, &joinpartydialog::onConnectedSuccessfully);
    connect(m_networkManager, &NetworkManager::connectionError, this, &joinpartydialog::onConnectionError);
    connect(m_networkManager, &NetworkManager::disconnectedFromPeer, this, [this](){
        ui->lblStatus->setText("Disconnected");
        ui->lblServerName->clear();
        ui->txtIP->setEnabled(true);
        ui->txtPort->setEnabled(true);
        ui->btnConnectToServer->setEnabled(true);
        ui->btnDisconnectFromServer->setEnabled(false);
    });


    connect(m_networkManager, &NetworkManager::peerIdentified, this, &joinpartydialog::onHostIdentified);

    ui->btnDisconnectFromServer->setEnabled(false);
}

joinpartydialog::~joinpartydialog()
{
    delete ui;
}

void joinpartydialog::on_btnConnectToServer_clicked()
{
    QString ip = ui->txtIP->text();
    quint16 port = ui->txtPort->text().toUShort();

    if (ip.isEmpty() || port == 0) {
        ui->lblStatus->setText("IP and Port are required.");
        return;
    }

    ui->btnConnectToServer->setEnabled(false);
    ui->btnDisconnectFromServer->setEnabled(true);
    ui->lblStatus->setText("Connecting...");

    m_networkManager->connectToPeer(QHostAddress(ip), port);
}

void joinpartydialog::on_btnDisconnectFromServer_clicked()
{
    m_networkManager->disconnectFromPeer();
    this->close();
}

void joinpartydialog::onConnectedSuccessfully() {
    ui->lblStatus->setText("Connected!");
    ui->txtIP->setEnabled(false);
    ui->txtPort->setEnabled(false);
    ui->btnConnectToServer->setEnabled(false);
    ui->btnDisconnectFromServer->setEnabled(true);
}

void joinpartydialog::onConnectionError(const QString &error) {
    ui->lblStatus->setText("Error: " + error);
    ui->txtIP->setEnabled(true);
    ui->txtPort->setEnabled(true);
    ui->btnConnectToServer->setEnabled(true);
    ui->btnDisconnectFromServer->setEnabled(false);
}


void joinpartydialog::onHostIdentified(const QString &username)
{
    ui->lblServerName->setText("Host: " + username);
}
