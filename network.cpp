#include "network.h"

network::network() {}


QNetworkReply *network::send_request(QByteArray method, QUrl url, QStringList headers, QByteArray data){
    QNetworkAccessManager *manager = new QNetworkAccessManager;
    QNetworkRequest request;

    request.setUrl(url);

    for(const auto &header : headers){
        QByteArray headername = header.split(",").at(0).toUtf8();

        QStringList splits = header.split(",");
        splits.removeFirst();
        QByteArray value;
        for (auto split : splits) {
            value.append(split.toUtf8());
            value.append(",");
        }
        request.setRawHeader(headername, value.mid(0, value.size()-1));
    }

    return manager->sendCustomRequest(request, method, data);
}
