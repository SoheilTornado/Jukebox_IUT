#include "registerdialog.h"
#include "ui_registerdialog.h"
#include <QMessageBox>
#include "UserManager.h"
registerdialog::registerdialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::registerdialog)
{
    ui->setupUi(this);
}

registerdialog::~registerdialog()
{
    delete ui;
}

void registerdialog::on_btnRegister_clicked()
{
    QString username = ui->txtUsername->text().trimmed();
    QString name = ui->txtName->text().trimmed();
    QString mail = ui->txtMail->text().trimmed();
    QString password = ui->txtPassword->text();
    QString hint = ui->txtHint->text().trimmed();


    // 1. Check for empty fields
    if (username.isEmpty() || name.isEmpty() || mail.isEmpty() || password.isEmpty()|| hint.isEmpty()) {
        QMessageBox::warning(this, "Error", "Please fill in all fields.");
        return;
    }
    UserManager::loadFromFile();
    // 2. Check if username already exists
    if (UserManager::usernameExists(username)) {
        QMessageBox::warning(this, "Error", "Username already exists.");
        return;
    }

    // 3. Hash the password
    QString hashedPassword = UserManager::hashPassword(password);

    // 4. Create new user
    User newUser;
    newUser.username = username;
    newUser.name = name;
    newUser.mail = mail;
    newUser.passwordHash = hashedPassword;
    newUser.hint = hint;

    // 5. Add to UserManager and save to file
    UserManager::addUser(newUser);

    // 6. Show success message
    QMessageBox::information(this, "Success", "Registration successful.");

    // 7. Close the dialog
    this->accept();
}

