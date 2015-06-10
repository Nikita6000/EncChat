#include "mainmenu.h"
#include "ui_mainmenu.h"

MainMenu::MainMenu(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainMenu)
{
    ui->setupUi(this);
}

MainMenu::~MainMenu()
{
    delete ui;
}

void MainMenu::on_pbConnect_clicked()
{
    emit Connect();
}


void MainMenu::on_pbSettings_clicked()
{
    emit ChangeSettings();
}

void MainMenu::on_pbExit_clicked()
{
    exit(0);
}


