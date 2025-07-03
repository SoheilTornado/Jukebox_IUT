#include "welcomewindow.h"
#include "ui_welcomewindow.h"
#include "registerdialog.h"
#include "logindialog.h"
#include "./ui_welcomewindow.h"
#include "mainwindow.h"
welcomewindow::welcomewindow(QWidget *parent)
    :QMainWindow(parent)
    ,ui(new Ui::welcomewindow)
{
    ui->setupUi(this);
}

welcomewindow::~welcomewindow()
{
    delete ui;
}

void welcomewindow::on_btnLogin_clicked()
{
    logindialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        MainWindow *mainApp = new MainWindow(dialog.getUsername());
        mainApp->setWindowIcon(QIcon(":/icon/logo.svg"));
        mainApp->setWindowTitle("Jukebox_IUT");
        mainApp->show();
        this->close();
    }

}
void welcomewindow::on_btnRegister_clicked()
{
    registerdialog dialog(this);
    dialog.exec();
}


void welcomewindow::on_btnExit_clicked()
{
    this->close();
}

