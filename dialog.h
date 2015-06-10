#ifndef DIALOG_H
#define DIALOG_H

#include "include.h"

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();

private:
    Ui::Dialog *ui;
public slots:
    void on_pbConnect_clicked();
    void on_pbExit_clicked();
signals:
    void Connect(QHostAddress, quint16);
    void BackToMenu();
};

#endif // DIALOG_H
