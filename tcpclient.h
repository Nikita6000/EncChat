#ifndef TCPClient_H
#define TCPClient_H

#include "include.h"
#include "utilityclasses.h"

class MyClient : public QDialog
{
    Q_OBJECT
public:
    static const quint8 MESSAGE = 1;
    static const quint8 UNSECURED_CONNECTION = 2;
    static const quint8 SECURITY_CHECK = 3;
    MyClient(QWidget *parent = 0);
    void SetPort(uint Port){nPort = Port;}
    uint GetPort(void){return nPort;}
    bool isConnectionSecured()
    {
        return ConnectionSecurityStatus;
    }
private:
    QTcpSocket *_socket;    // Сокет
    QTcpServer *_server;
    MyDownloader *IpDownloader;
    quint16 _blockSize = 0;     // Текущий размер блока данных
    bool ConnectionSecurityStatus;  // TRUE - ключи для шифрования созданы и проверены
                                    // FALSE - ключи не были сгенерированы или проверка
                                    // шифрования заданой строки выявила ошибку
    bool isConnectionExist = 0; // По задумке чат может быть только 1 на 1, т.о. если
                                // isConnectionExist = 1, то при запросе на соединение
                                // отправится сообщение о занятости юзверя
    uint nPort = 9100;
    // Отправляем сообщение клиенту отсюда
    void SendToClient (QTcpSocket *socket, const QString *str);
private slots:
// Слоты "клиента":
    void SocketConnected();
    void SocketDisconnected();
    // Cигнал readyRead вызывается, когда сокет получает пакет
    // (который может быть лишь частью отправленых данных) байтов
    void SocketReadyRead();
    void SocketDisplayError(QAbstractSocket::SocketError socketError);
// Слоты "сервера":
    // Создаем соединения тут
    void ServerNewConnection();
    // Показ ошибки при соединении
    void ShowServerError(QAbstractSocket::SocketError serverError);
public slots:
    // Слот Connect получает сигнал (с данными) от Dialog для создания подключения
    void Connect(QHostAddress IP, quint16 Port);
    // Сюда приходит зашифрованное сообщение для отправки
    void SendMessageOut (const QString *message);
    // Сюда приходит запрос на показ IP
    void RequestServerIP();
signals:
    // Сигнал для MainWindow c зашифрованным сообщением
    void NewMessage(const QString *message);
    // Сигнал установки соединения (закрывает Dialog'овое окно)
    void ConnectionValid();
    // Выведем IP на экран
    void ShowIP(QHostAddress);
};

#endif // TCPClient_H

