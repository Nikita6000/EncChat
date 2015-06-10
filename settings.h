#ifndef SETTINGS_H
#define SETTINGS_H

#include "include.h"

namespace Ui {
class Settings;
}

class Settings : public QWidget
{
    Q_OBJECT

public:
    explicit Settings(QWidget *parent = 0);
    ~Settings();

private:
    Ui::Settings *ui;
public slots:
    void ShowSettings(const QString Name, uint Port);
    void on_pbSave_clicked();
    void on_pbCancel_clicked();
    void ShowIP(QHostAddress IP);
signals:
    void NewSettings(const QString Name, uint Port);
    void Cancel();
};

#endif // SETTINGS_H
