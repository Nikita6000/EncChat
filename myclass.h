#ifndef MYCLASS_H
#define MYCLASS_H

#include "tcpclient.h"


class MyClass: public QObject
{
    Q_OBJECT
private:
    QString UserName;
    QFile Log;
    QTextStream out;
public:
    explicit MyClass(QObject *parent = 0);
    ~MyClass();
    void SetUserName(const QString *User_Name);
    QString GetUserName(void){return UserName;}
public slots:
    void WriteToLog(const QString *str);  // записывает все в log файл
    void ReceiveMessage(const QString *str); // Получает сообщение для шифровки и передачи собеседнику
    void ReceiveEncryptedMessage(const QString *str);// Получает сообщение для расшифровки и вывода
signals:
    void SendMessageToClient(const QString *str);  // Пересылает шифрованное сообщение
    void ShowMessage(const QString str);   // Вывод сообщения
    void LogMessage(const QString *str);
    void EncryptionFinished();          // Окончание шифровки и отпрвки
    void DecryptionFinished();          // Окончание расшифровки и вывода
    void EndLog();                         // Окончание записи log
};

#endif // MYCLASS_H

