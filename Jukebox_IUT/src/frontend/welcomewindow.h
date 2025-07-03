#ifndef WELCOMEWINDOW_H
#define WELCOMEWINDOW_H

#include <QMainWindow>

namespace Ui {
class welcomewindow;
}

class welcomewindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit welcomewindow(QWidget *parent = nullptr);
    ~welcomewindow();

private slots:
    void on_btnLogin_clicked();

    void on_btnRegister_clicked();

    void on_btnExit_clicked();

private:
    Ui::welcomewindow *ui;
};

#endif // WELCOMEWINDOW_H
