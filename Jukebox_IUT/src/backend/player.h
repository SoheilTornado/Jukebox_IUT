#ifndef PLAYER_H
#define PLAYER_H

#include <QMediaPlayer>
#include <QAudioOutput>
#include <QFileDialog>
#include <QStyle>

class player final : public QMediaPlayer
{
    Q_OBJECT
public:
    static player* getInstance();
    void toggleMute();
    QAudioOutput* audioOutput() const;

signals:
    void muteStateChanged(bool muted);

private:
    explicit player();
    static player* instance;
    QAudioOutput* m_audioOutput;
};

#endif //PLAYER_H
