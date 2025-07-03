#include "networkmanager.h"
#include <QtNetwork>
#include <QJsonDocument>

constexpr quint16 TCP_PORT = 55001;

NetworkManager::NetworkManager(QObject *parent) : QObject(parent) {
    m_tcpServer = new QTcpServer(this);
    connect(m_tcpServer, &QTcpServer::newConnection, this, &NetworkManager::onNewConnection);
}

void NetworkManager::startServer() {
    if (!m_tcpServer->listen(QHostAddress::Any, TCP_PORT)) {
        emit connectionError(m_tcpServer->errorString());
        return;
    }
    qDebug() << "Server started on port" << TCP_PORT;
}

void NetworkManager::connectToPeer(const QHostAddress &address, quint16 port) {
    if (m_peerSocket) m_peerSocket->deleteLater();
    m_peerSocket = new QTcpSocket(this);
    connect(m_peerSocket, &QTcpSocket::readyRead, this, &NetworkManager::onReadyRead);
    connect(m_peerSocket, &QAbstractSocket::stateChanged, this, &NetworkManager::onSocketStateChanged);
    connect(m_peerSocket, &QAbstractSocket::errorOccurred, this, &NetworkManager::onSocketErrorOccurred);
    m_peerSocket->connectToHost(address, port);
}

void NetworkManager::disconnectFromPeer() {
    if (m_tcpServer->isListening()) m_tcpServer->close();
    if (m_peerSocket) m_peerSocket->disconnectFromHost();
}

void NetworkManager::onNewConnection() {
    if (m_peerSocket) {
        m_tcpServer->nextPendingConnection()->close();
        return;
    }
    m_peerSocket = m_tcpServer->nextPendingConnection();
    connect(m_peerSocket, &QTcpSocket::readyRead, this, &NetworkManager::onReadyRead);
    connect(m_peerSocket, &QAbstractSocket::stateChanged, this, &NetworkManager::onSocketStateChanged);
    connect(m_peerSocket, &QAbstractSocket::errorOccurred, this, &NetworkManager::onSocketErrorOccurred);
    emit connectedToPeer();

    // معرفی خود به کلاینت
    QJsonObject payload;
    payload["username"] = m_localUsername;
    QJsonObject command;
    command["command"] = "IDENTIFY";
    command["payload"] = payload;
    sendCommand(command);
}

void NetworkManager::onReadyRead() {
    if (!m_peerSocket || !m_peerSocket->bytesAvailable()) return;
    m_buffer.append(m_peerSocket->readAll());
    processData();
}

void NetworkManager::processData() {
    while (true) {
        if (m_isFileTransferInProgress) {
            if (m_buffer.isEmpty()) break;
            qint64 bytesToWrite = m_receivingFile->write(m_buffer);
            m_bytesReceived += bytesToWrite;
            m_buffer.clear();

            if (m_bytesReceived >= m_fileSize) {
                emit fileReceivedSuccessfully(m_receivingFile->fileName());
                m_receivingFile->close();
                delete m_receivingFile;
                m_receivingFile = nullptr;
                m_isFileTransferInProgress = false;
            }
            break;
        } else {
            if (m_buffer.size() < sizeof(quint32)) break;
            QDataStream in(m_buffer.left(sizeof(quint32)));
            quint32 commandSize;
            in >> commandSize;

            if (m_buffer.size() < sizeof(quint32) + commandSize) break;

            m_buffer.remove(0, sizeof(quint32));
            QByteArray commandData = m_buffer.left(commandSize);
            m_buffer.remove(0, commandSize);

            QJsonObject command = QJsonDocument::fromJson(commandData).object();

            // پردازش فرمان IDENTIFY
            if (command["command"].toString() == "IDENTIFY") {
                m_peerUsername = command["payload"].toObject()["username"].toString();
                emit peerIdentified(m_peerUsername);
            }
            else if (command["command"].toString() == "SEND_SONG") {
                m_isFileTransferInProgress = true;
                m_fileSize = command["payload"].toObject()["file_size"].toVariant().toLongLong();
                m_bytesReceived = 0;
                QString fileName = QDir::tempPath() + "/" + command["payload"].toObject()["file_name"].toString();
                m_receivingFile = new QFile(fileName);
                if (!m_receivingFile->open(QIODevice::WriteOnly)) m_isFileTransferInProgress = false;
            } else {
                emit commandReceived(command);
            }
        }
    }
}

void NetworkManager::sendCommand(const QJsonObject &command) {
    if (!isConnected()) return;
    QByteArray commandData = QJsonDocument(command).toJson(QJsonDocument::Compact);
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out << (quint32)commandData.size();
    block.append(commandData);
    m_peerSocket->write(block);
}

void NetworkManager::sendFile(const QString &filePath) {
    if (!isConnected()) return;
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) return;

    QJsonObject payload;
    payload["file_name"] = QFileInfo(filePath).fileName();
    payload["file_size"] = file.size();
    QJsonObject command;
    command["command"] = "SEND_SONG";
    command["payload"] = payload;
    sendCommand(command);

    m_peerSocket->write(file.readAll());
}

void NetworkManager::onSocketStateChanged(QAbstractSocket::SocketState state) {
    if (state == QAbstractSocket::ConnectedState) {
        emit connectedToPeer();
        // معرفی خود به سرور
        QJsonObject payload;
        payload["username"] = m_localUsername;
        QJsonObject command;
        command["command"] = "IDENTIFY";
        command["payload"] = payload;
        sendCommand(command);
    } else if (state == QAbstractSocket::UnconnectedState) {
        if (m_peerSocket) {
            m_peerSocket->deleteLater();
            m_peerSocket = nullptr;
        }
        m_peerUsername.clear();
        m_buffer.clear();
        emit disconnectedFromPeer();
    }
}

void NetworkManager::onSocketErrorOccurred(QAbstractSocket::SocketError socketError) {
    Q_UNUSED(socketError);
    if (m_peerSocket) emit connectionError(m_peerSocket->errorString());
}

bool NetworkManager::isConnected() const {
    return m_peerSocket && m_peerSocket->state() == QAbstractSocket::ConnectedState;
}

void NetworkManager::setUsername(const QString &username) {
    m_localUsername = username;
}

QString NetworkManager::peerUsername() const {
    return m_peerUsername;
}
