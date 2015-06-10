#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "myclass.h"
#include "dialog.h"
#include "mainmenu.h"
#include "settings.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
private:
    Ui::MainWindow *ui;

    MyClass *Encryptor;
    MyClass *Decryptor;
    MyClass *Log;

    QThread *_Encryptor;
    QThread *_Decryptor;
    QThread *_Log;

    Dialog *ChooseAdress;
    MainMenu *Menu;
    Settings *SettingsDialog;

    MyClient *Client;
public slots:
    void on_pbSend_clicked();
    void MessageCreated();
    void MessageReceived(const QString *str);
    void NewSettings(const QString Name, uint Port);
    void ShowSettings();
signals:
    void SendToLog(const QString *str);
    void EncryptMessage(const QString *str);
    void SendSettings(const QString Name, uint Port);
};

#endif // MAINWINDOW_H
