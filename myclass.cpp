#include "mainwindow.h"

MyClass::MyClass(QObject *parent) :
    QObject(parent)
{
    // В случае
    if(UserName.isEmpty())
        MyClass::UserName.append("Alice");

    // Задаем имя лог-файла комбинацией имени пользователя, даты и времени
    QString str = UserName + " " + QDate::currentDate().toString()
                           + " " + QString::number(QTime::currentTime().hour())
                           + " " + QString::number(QTime::currentTime().minute())
                           + " " + QString::number(QTime::currentTime().second())
                           + ".log";

    Log.setFileName(str);
    if(!Log.open(QIODevice::WriteOnly))
    {
        qDebug() << "Ошибка открытия файла: " << Log.error();
    }
    else
    {
        // Задаем поток вывода в файл "out"
        out.setDevice(&Log);
    }
}

MyClass::~MyClass()
{
    emit EndLog();
    // Закрываем файл
    Log.close();
    // Очищаем имя пользователя
    UserName.clear();
}

void MyClass::SetUserName(const QString *User_Name)
{
    UserName.clear();
    UserName.append(User_Name);
}

void MyClass::WriteToLog(const QString *str)
{
    // todo: странная проблема с логом (либо крашится, либо пишет не текст, а байты)
    if(!str->isNull())
        out << str->toUtf8() << "\r\n";
}

void MyClass::ReceiveEncryptedMessage(const QString *str)
{
    QString *Str;
    Str = new QString;
    *Str = QString("%1 (incoming message)").arg(*str);  // На самом деле Str здесь для
                                                    // преобразования адреса. Сюда доходит
                                                // (видимо) указатель на str из слота
                                            // SocketReadyRead, который получается из
                                        // чтения QbyteArray. Хз почему, но если из слота
                                    // WriteToLog обратится к любому методу объекта str
                                // то приложение крашится с исключением "read violation"
                            // В итоге получается работать только с *str. Зато теперь
                                    // входящие сообщения в логе тоже обозначаются явно

    //todo: Расшифровка

    emit ShowMessage(QString("<font color=blue>%1</font>").arg(*str));
    emit LogMessage(Str);
    emit DecryptionFinished();
}

void MyClass::ReceiveMessage(const QString *str)
{
    //todo: Шифрование

    emit SendMessageToClient(str);
    emit EncryptionFinished();
}

