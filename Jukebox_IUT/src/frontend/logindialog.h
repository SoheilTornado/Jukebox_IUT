#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>

namespace Ui {
class logindialog;
}

class logindialog : public QDialog
{
    Q_OBJECT

public:
    explicit logindialog(QWidget *parent = nullptr);
    ~logindialog();
    QString getUsername() const;

private slots:
    void on_btnLogin_clicked();

    void on_btnForget_clicked();

private:
    Ui::logindialog *ui;
    QString m_username;

};

#endif // LOGINDIALOG_H
