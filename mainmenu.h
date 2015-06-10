#ifndef MAINMENU_H
#define MAINMENU_H

#include "include.h"

namespace Ui {
class MainMenu;
}

class MainMenu : public QWidget
{
    Q_OBJECT

public:
    explicit MainMenu(QWidget *parent = 0);
    ~MainMenu();

private:
    Ui::MainMenu *ui;
signals:
    void Connect();
    void Exit();
    void ChangeSettings();
public slots:
    void on_pbConnect_clicked();
    void on_pbSettings_clicked();
    void on_pbExit_clicked();
};

#endif // MAINMENU_H
