#include "createserverdialog.h"
#include "ui_createserverdialog.h"
#include "../backend/networkmanager.h"
#include <QNetworkInterface>

createserverdialog::createserverdialog(NetworkManager *networkManager, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::createserverdialog)
    , m_networkManager(networkManager)
{
    ui->setupUi(this);
    setWindowTitle("Party Host");
    setupServerInfo();


    connect(m_networkManager, &NetworkManager::connectedToPeer, this, &createserverdialog::updateClientList);
    connect(m_networkManager, &NetworkManager::disconnectedFromPeer, this, &createserverdialog::updateClientList);


    connect(m_networkManager, &NetworkManager::peerIdentified, this, &createserverdialog::onClientIdentified);
}

createserverdialog::~createserverdialog()
{
    delete ui;
}

void createserverdialog::on_btnCloseParty_clicked()
{
    m_networkManager->disconnectFromPeer();
    this->close();
}

void createserverdialog::setupServerInfo()
{
    ui->lblIP->setText(getLocalIpAddress());
    ui->lblPort->setText("55001");
    updateClientList();
}

void createserverdialog::updateClientList()
{
    ui->listUsers->clear();
    if (m_networkManager->isConnected()) {
        QString peerName = m_networkManager->peerUsername();
        if (peerName.isEmpty()) {
            ui->listUsers->addItem("User connecting...");
        } else {
            ui->listUsers->addItem(peerName + " is connected.");
        }
    } else {
        ui->listUsers->addItem("Waiting for users to connect...");
    }
}


void createserverdialog::onClientIdentified(const QString &username)
{
    ui->listUsers->clear();
    ui->listUsers->addItem(username + " is connected.");
}

QString createserverdialog::getLocalIpAddress()
{
    const QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
    for (const QHostAddress &addr : ipAddressesList) {
        if (addr != QHostAddress::LocalHost && addr.toIPv4Address()) {
            return addr.toString();
        }
    }
    return "127.0.0.1";
}
