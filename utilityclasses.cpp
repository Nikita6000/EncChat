#include "utilityclasses.h"

MyDownloader::MyDownloader(QObject *parent):
    QObject(parent)
{
    manager = new QNetworkAccessManager(this);

    connect(manager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(replyFinished(QNetworkReply*)));
}

MyDownloader::~MyDownloader()
{

}

void MyDownloader::doDownload()
{
    manager->get(QNetworkRequest(QUrl("http://ip-address.ru/show")));
}

void MyDownloader::replyFinished (QNetworkReply *reply)
{
    emit IPDownloaded(QHostAddress(QString(reply->readLine())));
}

