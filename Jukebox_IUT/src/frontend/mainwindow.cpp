#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "../backend/queueManager.h"
#include "controls.h"
#include "../backend/networkmanager.h"
#include "../backend/player.h"
#include "partydialog.h"
#include "joinpartydialog.h"
#include "createserverdialog.h"
#include "switcher.h"
#include "queue.h"
#include "playlists.h"
#include "../backend/playlistManager.h"
#include "welcomewindow.h"
#include "fakevisualizerwidget.h"

#include <QDialog>
#include <QCryptographicHash>
#include <QProcess>
#include <QApplication>
#include <QTimer>
#include <QDebug>
#include <QJsonDocument>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSplitter>
#include <QWidget>

MainWindow::MainWindow(const QString &username, QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), m_loggedInUsername(username)
{
    setAttribute(Qt::WA_DeleteOnClose);
    ui->setupUi(this);
    //Backend
    m_player = player::getInstance();
    m_networkManager = new NetworkManager(this);
    m_networkManager->setUsername(m_loggedInUsername);

    playlistManager::getInstance()->setUsername(username);
    queueManager::getInstance()->setUsername(username);

    //UI

    m_controls = new controls(this);
    switcher* viewSwitcher = switcher::getInstance();
    queue* queueView = new queue(this);
    playlists* playlistsView = new playlists(this);

    QWidget *centralWidget = new QWidget(this);
    this->setCentralWidget(centralWidget);

    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    QHBoxLayout *contentLayout = new QHBoxLayout();

    QSplitter *splitter = new QSplitter(Qt::Horizontal, this);
    splitter->addWidget(playlistsView);
    splitter->addWidget(viewSwitcher);
    splitter->addWidget(queueView);

    splitter->setSizes({250, 440, 250});

    contentLayout->addWidget(splitter);
    mainLayout->addLayout(contentLayout, 1);
    mainLayout->addWidget(m_controls);

    //splitter->setHandleWidth(5);
    splitter->setStyleSheet("QSplitter::handle { background-color: #333333; }");

    //eq


    m_visualizerDialog = new QDialog(this);
    m_visualizerDialog->setWindowTitle("Visualizer");
    m_visualizerWidget = new FakeVisualizerWidget(m_visualizerDialog);

    QVBoxLayout* vizLayout = new QVBoxLayout(m_visualizerDialog);
    vizLayout->addWidget(m_visualizerWidget);
    m_visualizerDialog->setLayout(vizLayout);
    m_visualizerDialog->setAttribute(Qt::WA_DeleteOnClose, false);


    connect(m_player, &player::muteStateChanged, m_visualizerWidget, &FakeVisualizerWidget::setMuted);
    connect(m_player, &QMediaPlayer::playbackStateChanged, this, [this](QMediaPlayer::PlaybackState state){
        if (state == QMediaPlayer::PlayingState) {
            m_visualizerWidget->startAnimation();
        } else { // Paused or Stopped
            m_visualizerWidget->stopAnimation();
        }
    });
    connect(m_controls, &controls::volumeChanged, m_visualizerWidget, &FakeVisualizerWidget::setMasterVolume);




    connect(m_networkManager, &NetworkManager::commandReceived, this, &MainWindow::onCommandReceived);
    connect(m_networkManager, &NetworkManager::fileReceivedSuccessfully, this, &MainWindow::onFileReceived);

    if(m_player) {
        connect(m_player, &QMediaPlayer::playbackStateChanged, this, &MainWindow::onPlayerStateChanged);
        connect(m_player, &QMediaPlayer::sourceChanged, this, &MainWindow::onNewSongSelected);

    }

    if (m_controls) {
        connect(m_controls, &controls::seekPositionChanged, this, &MainWindow::onSeekPositionChanged);
    }
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_actionParty_triggered() {
    partydialog dialog(this);
    int result = dialog.exec();

    if (result == partydialog::createParty) {

        createserverdialog createDialog(m_networkManager, this);
        m_networkManager->startServer();
        createDialog.exec();

    }
    else if (result == partydialog::joinParty) {
        joinpartydialog joinDialog(m_networkManager, this);
        joinDialog.exec();
    }
}

void MainWindow::onSeekPositionChanged(qint64 position) {
    if (m_networkManager->isConnected() && !m_isHandlingNetworkCommand) {
        QJsonObject payload;
        payload["position"] = position;
        QJsonObject command;
        command["command"] = "SEEK";
        command["payload"] = payload;
        m_networkManager->sendCommand(command);
    }
}

void MainWindow::onCommandReceived(const QJsonObject &command)
{
    m_isHandlingNetworkCommand = true;

    QString commandType = command["command"].toString();
    QJsonObject payload = command["payload"].toObject();

    if (commandType == "PLAY") {
        m_player->play();
        qDebug() << "Received PLAY command";
    }
    else if (commandType == "PAUSE") {
        m_player->pause();
        qDebug() << "Received PAUSE command";
    }
    else if (commandType == "SEEK") {
        qint64 position = payload["position"].toVariant().toLongLong();
        m_player->setPosition(position);
        qDebug() << "Received SEEK command to" << position;
    }
    else if (commandType == "NEW_SONG") {

        m_currentSongUrl = QUrl(payload["url"].toString());
        qDebug() << "Received NEW_SONG command for URL:" << m_currentSongUrl.toString() << ". Waiting for file...";
    }

    QTimer::singleShot(100, this, [this]() {
        m_isHandlingNetworkCommand = false;
    });
}

void MainWindow::onFileReceived(const QString &filePath)
{
    qDebug() << "File received successfully at:" << filePath;
    m_player->setSource(QUrl::fromLocalFile(filePath));
    m_player->play();


    m_isHandlingNetworkCommand = true;
    QTimer::singleShot(100, this, [this]() {
        m_isHandlingNetworkCommand = false;
    });
}

void MainWindow::onPlayerStateChanged(QMediaPlayer::PlaybackState state)
{
    if (m_networkManager->isConnected() && !m_isHandlingNetworkCommand) {
        QJsonObject command;
        if (state == QMediaPlayer::PlayingState) {
            command["command"] = "PLAY";
        } else if (state == QMediaPlayer::PausedState || state == QMediaPlayer::StoppedState) {
            command["command"] = "PAUSE";
        }
        m_networkManager->sendCommand(command);
    }
}



void MainWindow::onNewSongSelected(const QUrl &url)
{
    if (m_networkManager->isConnected() && !m_isHandlingNetworkCommand) {
        m_currentSongUrl = url;
        QJsonObject payload;
        payload["url"] = url.toString();
        payload["hash"] = getFileHash(url.toLocalFile());

        QJsonObject command;
        command["command"] = "NEW_SONG";
        command["payload"] = payload;

        qDebug() << "Sending NEW_SONG command for URL:" << url.toString();
        m_networkManager->sendCommand(command);

        if (url.isLocalFile()) {
            m_networkManager->sendFile(url.toLocalFile());
        }
    }
}
void MainWindow::onPlayerPositionChanged(qint64 position)
{

    Q_UNUSED(position);
}
QString MainWindow::getFileHash(const QString &filePath) const
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) return "";
    QCryptographicHash hash(QCryptographicHash::Md5);
    if (hash.addData(&file)) {
        return hash.result().toHex();
    }
    return "";
}

void MainWindow::on_actionLogout_triggered()
{
    QProcess::startDetached(QApplication::applicationFilePath());

    QApplication::quit();
}


void MainWindow::on_actionExit_triggered()
{
    QApplication::quit();
}


void MainWindow::on_actionVisualizer_triggered()
{
    if (m_visualizerDialog) {
        if (m_visualizerDialog->isVisible()) {
            m_visualizerDialog->hide();
        } else {
            m_visualizerDialog->show();
        }
    }
}

