#include "logindialog.h"
#include "ui_logindialog.h"
#include <QMessageBox>
#include "UserManager.h"
logindialog::logindialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::logindialog)
{
    ui->setupUi(this);
    ui->lblHint->setVisible(false);
}

logindialog::~logindialog()
{
    delete ui;
}

QString logindialog::getUsername() const
{
    return m_username;
}

void logindialog::on_btnLogin_clicked()
{
    QString username = ui->txtUsername->text().trimmed();
    QString password = ui->txtPassword->text();

    if (username.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this,"login error","Error . username and password cannot be empty;");
        return;
    }

    QString hashed = UserManager::hashPassword(password);
    UserManager::loadFromFile();
    if (UserManager::validateLogin(username, hashed)) {
        m_username = username;
        QMessageBox::information(this,"successful login","login successful you can continue");
        this->accept();
    } else {
        QMessageBox::critical(this, "login error","Error . password incorrect or invalid account. if you've forgotten pass,remember by hint");
    }
}


void logindialog::on_btnForget_clicked()
{
    UserManager::loadFromFile();
    QString username = ui->txtUsername->text().trimmed();

    if (ui->lblHint->isVisible()) {
        ui->lblHint->setText("");
        ui->lblHint->setVisible(false);
        ui->btnForget->setText("Forget password");
    }
    else {
        if(UserManager::usernameExists(username)){
            QString text = "Hint: " + UserManager::getHint(username);
            ui->lblHint->setText(text);
            ui->btnForget->setText("Click to Hide");
            ui->lblHint->setVisible(true);
        }
        else{
            QMessageBox::warning(this, "Error", "Username doesn't exist.");
        }
    }

}

