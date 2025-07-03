#ifndef QUEUE_H
#define QUEUE_H

#include <QWidget>


QT_BEGIN_NAMESPACE
namespace Ui { class queue; }
QT_END_NAMESPACE

class queue final : public QWidget {
Q_OBJECT

public:
    explicit queue(QWidget *parent = nullptr);
    ~queue() override;

private slots:
    void onQueueChanged();

private:
    Ui::queue *ui;

    void renderSongs();
};


#endif //QUEUE_H
