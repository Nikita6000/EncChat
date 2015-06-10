#include "tcpclient.h"
//#include "utilityclasses.h"

MyClient::MyClient(QWidget *parent):
    QDialog(parent)
{
    // Создаем сокет
    _socket = new QTcpSocket(this);
    // Создаем сервер
    _server = new QTcpServer(this);

    IpDownloader = new MyDownloader;

    // Проверка сервера
    if(!_server->listen(QHostAddress::Any, nPort))
    {
        if(!_server->listen(QHostAddress::Any, nPort+10))
        {
            QMessageBox::critical(0, "Server Error",
                              "Unable to start the server: "
                              + _server->errorString());
            _server->close();
            return;
        }
    }
    // Подключаем сигналы "сервера"
    connect(_server, SIGNAL(newConnection()), this, SLOT(ServerNewConnection()));
    connect(_server, SIGNAL(acceptError(QAbstractSocket::SocketError)),
                  this, SLOT(ShowServerError(QAbstractSocket::SocketError)));
    // Подключаем сигналы "клиента"
    connect(_socket, SIGNAL(readyRead()), this, SLOT(SocketReadyRead()));
    connect(_socket, SIGNAL(connected()), this, SLOT(SocketConnected()));
    connect(_socket, SIGNAL(disconnected()), this, SLOT(SocketDisconnected()));
    connect(_socket, SIGNAL(error(QAbstractSocket::SocketError)),this,
                        SLOT(SocketDisplayError(QAbstractSocket::SocketError)));

    connect(IpDownloader, SIGNAL(IPDownloaded(QHostAddress)), this,
                                            SIGNAL(ShowIP(QHostAddress)));
}

void MyClient::Connect(QHostAddress IP, quint16 Port)
{
    _socket->connectToHost(IP, Port);
    if(_socket->isValid())
    {
        isConnectionExist = 1;
        emit ConnectionValid();
    }
    else
    {
        QMessageBox::critical(0, "Socket Error",
                              "Unable to create a socket: "
                              + _socket->errorString());
        _socket->abort();
        return;
    }
}

void MyClient::ServerNewConnection()
{
    QTcpSocket *ClientSocket = _server->nextPendingConnection();

    // Если одно соединение уже существует, то вежливо извиняемся и обрываем соединение
    if(isConnectionExist)
    {
        QString Apologizing_Message = "This user is alredy chating with someone(";
        SendToClient(ClientSocket, &Apologizing_Message);
        ClientSocket->disconnectFromHost();
        return;
    }
    else
    {
        QString Message = "Connection established!";
        SendToClient(ClientSocket, &Message);
    }
    // Теперь _socket соединен с запрашивававшимм соединение
    _socket = ClientSocket;

    connect(_socket, SIGNAL(readyRead()), this, SLOT(SocketReadyRead()));
    connect(_socket, SIGNAL(connected()), this, SLOT(SocketConnected()));
    connect(_socket, SIGNAL(disconnected()), this, SLOT(SocketDisconnected()));

    if(_socket->isValid())
    {
        isConnectionExist = 1;
        emit ConnectionValid();
    }
    else
    {
        QMessageBox::critical(0, "Socket Error",
                              "Unable to create a socket: "
                              + _socket->errorString());
        _socket->abort();
        return;
    }
}

void MyClient::SendToClient(QTcpSocket *socket, const QString *str)
{
    // Если считаем, что соединение не безопасно
    if(!ConnectionSecurityStatus)
    {
        // todo: Сигнал на новый обмен кодами шифрования
    }
    MyData x;
    x.mode = (quint8)MESSAGE;
    // можно любым способом в QByteArray QString здесь завернуть.
    x.Message = str->toUtf8();

    // Наконец отправляем сообщение
    socket->write(x.serialize());
}

void MyClient::SocketConnected()
{
    // После подкючения необходимо сгенерировать код для шифрования
    MyData x;
    x.mode = (quint8)UNSECURED_CONNECTION;

    // x.Message = ;
    // todo: Запихать в message числа для генерации шифра

    if(_socket->write(x.serialize()) == -1)
    {
        QMessageBox::critical(this, "Error",
                              "Error while writing in socket");
    }
}

void MyClient::SocketDisconnected()
{
    isConnectionExist = 0;
    _socket->disconnectFromHost();
    _server->close();
}

void MyClient::SocketDisplayError(QAbstractSocket::SocketError socketError)
{
    switch(socketError)
    {
    default:
        QMessageBox::critical(0, "Socket Error!", tr("Code: %1").arg(socketError));
        break;
    case 1:
        // Разрыв соединения. todo: Возврат в главное меню
        break;
    }
}

void MyClient::ShowServerError(QAbstractSocket::SocketError serverError)
{
    QMessageBox::critical(0, "Server Error!", tr("Code: %1").arg(serverError));
}

void MyClient::SendMessageOut (const QString *message)
{
   emit SendToClient(_socket, message);
}

void MyClient::SocketReadyRead()
{
//тут обрабатываются данные от сервера
    QTcpSocket *Socket = qobject_cast<QTcpSocket*>(sender());
    MyData IncomingMessage;
//если считываем новый блок первые 2 байта это его размер

    if(!IncomingMessage.deserealize(Socket->readAll()))
        return;
    switch (IncomingMessage.mode)
    {
        default:
        case MyClient::UNSECURED_CONNECTION:
        {

            // todo: Обработку команды сервера о незащищенном канале (т.е. обмен ключами)
        }
        //общее сообщение от сервера
        case MyClient::MESSAGE:
        {
            QString str(IncomingMessage.Message);
            //Скидываем сообщение на расшифровку
            emit NewMessage(&str);
            break;
        }
    }
}

void MyClient::RequestServerIP()
{
    IpDownloader->doDownload();
}
