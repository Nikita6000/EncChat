#ifndef UTILITYCLASSES_H
#define UTILITYCLASSES_H

#include "include.h"

class MyData {
public:
    quint8 mode;
    QByteArray Message;
    QByteArray serialize()
    {
        quint16 size = this->Message.size() + 3;
        // место под размер
        QByteArray retVal(0x02, 0xff);
        qToLittleEndian<quint16>(size, (uchar*)retVal.data());
        retVal.append((char)this->mode);
        retVal.append(this->Message);
        return retVal;
    }
    bool deserealize(const QByteArray value)
    {
        if (value.size() < 3) {
            return false;
        }
        quint16 size = qFromLittleEndian<quint16>((const uchar*)value.constData());
        if (size != value.size()) {
            return false;
        }
        this->mode = (quint8)value.at(2);
        this->Message = value.mid(3);
        return true;
    }
};

class MyDownloader : public QObject
{
    Q_OBJECT
public:
    explicit MyDownloader(QObject *parent = 0);
    ~MyDownloader();
    void doDownload(void);
signals:
    void IPDownloaded(QHostAddress IP);
public slots:
    void replyFinished (QNetworkReply *reply);

private:
   QNetworkAccessManager *manager;

};

#endif // UTILITYCLASSES_H

