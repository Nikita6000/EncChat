#include "settings.h"
#include "ui_settings.h"

Settings::Settings(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Settings)
{
    ui->setupUi(this);
}

Settings::~Settings()
{
    delete ui;
}


void Settings::ShowSettings(const QString Name, uint Port)
{
    ui->tlName->setText(Name);
    ui->tlPort->setText(QString::number(Port));
}

void Settings::on_pbSave_clicked()
{
    emit NewSettings(ui->tlName->text(), ui->tlPort->text().toUInt());
}

void Settings::on_pbCancel_clicked()
{
    emit Cancel();
}


void Settings::ShowIP(QHostAddress IP)
{
    ui->lbIP->setText(QString("Ваш IP: %1").arg(IP.toString()));
}
