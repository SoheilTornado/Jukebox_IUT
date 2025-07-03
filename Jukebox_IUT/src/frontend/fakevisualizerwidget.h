#ifndef FAKEVISUALIZERWIDGET_H
#define FAKEVISUALIZERWIDGET_H

#include <QWidget>
#include <QTimer>
#include <QVector>

class FakeVisualizerWidget : public QWidget
{
    Q_OBJECT
public:
    explicit FakeVisualizerWidget(QWidget *parent = nullptr);

public slots:
    void startAnimation();
    void stopAnimation();
    void setMasterVolume(qreal volume);
    void setMuted(bool muted);

private slots:
    void onTimeout();

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QTimer *m_animationTimer;
    QVector<int> m_barHeights;
    int m_barCount = 30;
    qreal m_masterVolume = 1.0;
    bool m_isMuted = false;
};

#endif // FAKEVISUALIZERWIDGET_H
