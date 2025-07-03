#include "player.h"
#include <QMediaPlayer>

player *player::instance = nullptr;

player::player() {
    m_audioOutput = new QAudioOutput(this);
    setAudioOutput(m_audioOutput);


    connect(m_audioOutput, &QAudioOutput::mutedChanged, this, &player::muteStateChanged);
}

player *player::getInstance() {
    if (instance == nullptr) { instance = new player(); }
    return instance;
}

void player::toggleMute() {
    m_audioOutput->setMuted(!m_audioOutput->isMuted());
}

QAudioOutput* player::audioOutput() const {
    return m_audioOutput;
}
