#include "partydialog.h"
#include "ui_partydialog.h"

partydialog::partydialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::partydialog)
{
    ui->setupUi(this);
}

partydialog::~partydialog()
{
    delete ui;
}

void partydialog::on_btnCreateServer_clicked()
{
    done(createParty);
}


void partydialog::on_btnJoinServer_clicked()
{
    done(joinParty);
}

