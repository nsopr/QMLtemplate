#include "dropbox.h"

dropbox::dropbox() {
    connect(this, &dropbox::access_token_ready, this, &dropbox::get_note_list);
}

void dropbox::generate_token(){
    QUrl url("https://api.dropbox.com/oauth2/token");
    QByteArray data = "grant_type=refresh_token&refresh_token=" + ref_tok + "&client_id=" + app_key + "&client_secret=" + app_sec;

    QNetworkReply *reply = nw.send_request("POST", url, QStringList(), data);

    connect(reply, &QNetworkReply::finished, this, [this, reply]{
        QJsonObject jobj = QJsonDocument::fromJson(reply->readAll()).object();
        access_token = jobj["access_token"].toString().toUtf8();
        emit access_token_ready();
    });
}

void dropbox::list_file_folder(){
    QUrl url("https://api.dropboxapi.com/2/files/list_folder");
    QStringList headers;
    headers << "Authorization,Bearer " + access_token << "Content-Type,text/plain; charset=dropbox-cors-hack";

}

void dropbox::get_note_list(){

}
