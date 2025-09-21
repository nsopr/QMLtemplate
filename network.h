#ifndef NETWORK_H
#define NETWORK_H

#include <QtNetwork>

class network
{
public:
    network();

    QNetworkReply* send_request(QByteArray method, QUrl url, QStringList headers, QByteArray data);

};

#endif // NETWORK_H
