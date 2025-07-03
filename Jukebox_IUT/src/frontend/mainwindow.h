#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QJsonObject>
#include <QMediaPlayer>
#include "controls.h"
#include "fakevisualizerwidget.h"

class NetworkManager;
class player;
class controls;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow final : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(const QString &username, QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    void on_actionParty_triggered();

    void onCommandReceived(const QJsonObject &command);
    void onFileReceived(const QString &filePath);

    void onPlayerStateChanged(QMediaPlayer::PlaybackState state);
    void onPlayerPositionChanged(qint64 position);
    void onNewSongSelected(const QUrl &url);
    void onSeekPositionChanged(qint64 position);

    void on_actionLogout_triggered();

    void on_actionExit_triggered();

    void on_actionVisualizer_triggered();

private:
    Ui::MainWindow *ui;
    QString m_loggedInUsername;

    NetworkManager *m_networkManager;
    player *m_player;
    controls *m_controls;

    QUrl m_currentSongUrl;
    bool m_isHandlingNetworkCommand = false;

    QString getFileHash(const QString &filePath) const;

    QDialog* m_visualizerDialog;
    FakeVisualizerWidget* m_visualizerWidget;
};

#endif // MAINWINDOW_H
