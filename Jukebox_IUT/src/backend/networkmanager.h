#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <QObject>
#include <QHostAddress>
#include <QAbstractSocket>
#include <QJsonObject>
#include <QFile>

class QTcpServer;
class QTcpSocket;

class NetworkManager : public QObject
{
    Q_OBJECT

public:
    explicit NetworkManager(QObject *parent = nullptr);
    bool isConnected() const;
    QString peerUsername() const;

public slots:
    void startServer();
    void connectToPeer(const QHostAddress &address, quint16 port);
    void disconnectFromPeer();
    void sendCommand(const QJsonObject &command);
    void sendFile(const QString &filePath);
    void setUsername(const QString &username);

signals:
    void connectedToPeer();
    void disconnectedFromPeer();
    void connectionError(const QString &error);
    void commandReceived(const QJsonObject &command);
    void fileReceivedSuccessfully(const QString &filePath);
    void peerIdentified(const QString &username);

private slots:
    void onNewConnection();
    void onReadyRead();
    void onSocketStateChanged(QAbstractSocket::SocketState state);
    void onSocketErrorOccurred(QAbstractSocket::SocketError socketError);

private:
    void processData();

    QTcpServer *m_tcpServer = nullptr;
    QTcpSocket *m_peerSocket = nullptr;
    QByteArray m_buffer;

    // متغیرهای انتقال فایل
    bool m_isFileTransferInProgress = false;
    QFile *m_receivingFile = nullptr;
    qint64 m_fileSize = 0;
    qint64 m_bytesReceived = 0;

    // متغیرهای جدید برای نام کاربری
    QString m_localUsername;
    QString m_peerUsername;
};

#endif // NETWORKMANAGER_H
