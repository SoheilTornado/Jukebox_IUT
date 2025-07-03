#include "fakevisualizerwidget.h"
#include <QPainter>
#include <QRandomGenerator>
#include <QLinearGradient>

FakeVisualizerWidget::FakeVisualizerWidget(QWidget *parent) : QWidget(parent)
{
    setMinimumSize(320, 180);
    setAutoFillBackground(true);
    QPalette pal = palette();
    pal.setColor(QPalette::Window, Qt::black);
    setPalette(pal);

    m_animationTimer = new QTimer(this);
    connect(m_animationTimer, &QTimer::timeout, this, &FakeVisualizerWidget::onTimeout);

    m_barHeights.resize(m_barCount);
    m_barHeights.fill(0);
}

void FakeVisualizerWidget::startAnimation()
{
    if (!m_animationTimer->isActive()) {
        m_animationTimer->start(50);
    }
}

void FakeVisualizerWidget::stopAnimation()
{
    m_animationTimer->stop();
    m_barHeights.fill(0);
    update();
}

void FakeVisualizerWidget::setMasterVolume(qreal volume)
{
    m_masterVolume = volume;
}

void FakeVisualizerWidget::setMuted(bool muted)
{
    m_isMuted = muted;
}

void FakeVisualizerWidget::onTimeout()
{
    if (m_isMuted) {
        bool needsUpdate = false;
        for(int& barHeight : m_barHeights) { if (barHeight != 0) { barHeight = 0; needsUpdate = true; } }
        if (needsUpdate) update();
        return;
    }

    int availableHeight = static_cast<int>(height() * 0.90);
    int maxHeight = static_cast<int>(availableHeight * m_masterVolume);

    if (maxHeight < 5) {
        bool needsUpdate = false;
        for(int& barHeight : m_barHeights) { if (barHeight != 0) { barHeight = 0; needsUpdate = true; } }
        if (needsUpdate) update();
        return;
    }

    for (int i = 0; i < m_barCount; ++i) {
        if (QRandomGenerator::global()->bounded(4) == 0) {
            m_barHeights[i] = QRandomGenerator::global()->bounded(5, maxHeight);
        } else {
            m_barHeights[i] -= height() / 15;
        }
        if (m_barHeights[i] < 0) {
            m_barHeights[i] = 0;
        }
    }
    update();
}


void FakeVisualizerWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setPen(Qt::NoPen);

    QLinearGradient gradient(0, 0, 0, height());
    gradient.setColorAt(0.0, QColor(255, 0, 0));
    gradient.setColorAt(0.5, QColor(255, 255, 0));
    gradient.setColorAt(1.0, QColor(0, 255, 0));
    painter.setBrush(gradient);

    float slotWidth = static_cast<float>(width()) / m_barCount;
    float barWidth = slotWidth * 0.6f;
    float padding = (slotWidth - barWidth) / 2.0f;

    for (int i = 0; i < m_barCount; ++i) {
        float barX = (i * slotWidth) + padding;
        painter.drawRect(QRectF(barX, height() - m_barHeights[i], barWidth, m_barHeights[i]));
    }
}
