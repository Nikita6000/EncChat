#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
}

Dialog::~Dialog()
{
    delete ui;
}


void Dialog::on_pbConnect_clicked()
{
    emit Connect(QHostAddress(ui->tlIP->text()), ui->tlPort->text().toInt());
}

void Dialog::on_pbExit_clicked()
{
    emit BackToMenu();
}
